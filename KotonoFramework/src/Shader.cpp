#include "Shader.h"
#include "Framework.h"
#include "vk_utils.h"
#include "Context.h"
#include "ImageTextureManager.h"
#include "Path.h"
#include "File.h"
#include <spirv-reflect/spirv_reflect.h>
#include <nlohmann/json.hpp>
#include "Serializer.h"
#include "log.h"

KtShader::KtShader(const std::filesystem::path& path) :
	_path(path)
{
}

void KtShader::Init()
{
	KT_DEBUG_LOG("initializing shader '%s'", _name.c_str());
	CreateShaderLayout();
	CreateDescriptorSetLayouts();
	CreateDescriptorSetLayoutBindingBuffers();
	CreateDescriptorSetLayoutBindingImages();
	CreateDescriptorPools();
	CreateDescriptorSets();
	CreateGraphicsPipeline();
	DebugLogDescriptorSetLayoutData();
	KT_DEBUG_LOG("initialized shader '%s'", _name.c_str());
}

void KtShader::Cleanup()
{
	KT_DEBUG_LOG("cleaning up shader '%s'", _name.c_str());

	vkDestroyPipeline(Framework.GetContext().GetDevice(), _graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(Framework.GetContext().GetDevice(), _pipelineLayout, nullptr);

	for (const auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		for (const auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
			{
				vmaDestroyBuffer(Framework.GetContext().GetAllocator(), descriptorSetLayoutBindingData.Buffers[i].Buffer, descriptorSetLayoutBindingData.Buffers[i].Allocation);
				vmaDestroyBuffer(Framework.GetContext().GetAllocator(), descriptorSetLayoutBindingData.StagingBuffers[i].Buffer, descriptorSetLayoutBindingData.StagingBuffers[i].Allocation);
			}
		}

		vkDestroyDescriptorSetLayout(Framework.GetContext().GetDevice(), descriptorSetLayoutData.DescriptorSetLayout, nullptr);
	}

	vkDestroyDescriptorPool(Framework.GetContext().GetDevice(), _descriptorPool, nullptr);

	KT_DEBUG_LOG("cleaned up shader '%s'", _name.c_str());
}

const std::filesystem::path& KtShader::GetPath() const
{
	return _path;
}

const std::string& KtShader::GetName() const
{
	return _name;
}

void KtShader::SetName(const std::string& name)
{
	_name = name;
}

VkPipeline KtShader::GetGraphicsPipeline() const
{
	return _graphicsPipeline;
}

VkPipelineLayout KtShader::GetPipelineLayout() const
{
	return _pipelineLayout;
}

void KtShader::CmdBind(VkCommandBuffer commandBuffer) const
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);
}

void KtShader::CmdBindDescriptorSets(VkCommandBuffer commandBuffer, const uint32_t imageIndex) const
{
	std::vector<VkDescriptorSet> descriptorSets;
	descriptorSets.reserve(_descriptorSetLayoutDatas.size());
	for (const auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		descriptorSets.push_back(descriptorSetLayoutData.DescriptorSets[imageIndex]);
	}
	vkCmdBindDescriptorSets(
		commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout,
		0, static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), 0, nullptr
	);
}

void KtShader::CreateDescriptorSetLayouts()
{
	_descriptorSetLayoutDatas.reserve(_shaderLayout.DescriptorSetLayouts.size());
	for (const auto& [set, setLayout] : _shaderLayout.DescriptorSetLayouts)
	{
		std::vector<VkDescriptorSetLayoutBinding> setBindings;
		std::vector<DescriptorSetLayoutBindingData> setBindingDatas;
		setBindings.reserve(setLayout.DescriptorSetLayoutBindings.size());
		setBindingDatas.reserve(setLayout.DescriptorSetLayoutBindings.size());
		for (const auto& ktBinding : setLayout.DescriptorSetLayoutBindings)
		{
			VkDescriptorSetLayoutBinding vkBinding{};
			vkBinding.binding = ktBinding.Binding;
			vkBinding.descriptorCount = ktBinding.DescriptorCount;
			vkBinding.descriptorType = ktBinding.DescriptorType;
			vkBinding.stageFlags = ktBinding.StageFlags;
			vkBinding.pImmutableSamplers = nullptr; // Optional
			setBindings.push_back(vkBinding);

			DescriptorSetLayoutBindingData bindingData{};
			bindingData.Name = ktBinding.Name;
			bindingData.Binding = ktBinding.Binding;
			bindingData.MemberSize = ktBinding.Size;
			bindingData.DescriptorType = ktBinding.DescriptorType;
			bindingData.DescriptorCount = ktBinding.DescriptorCount;
			bindingData.StageFlags = ktBinding.StageFlags;
			bindingData.ImageTexture = nullptr; // Optional
			bindingData.MemberCounts.fill(1);
			setBindingDatas.push_back(bindingData);
		}
		VkDescriptorSetLayout newSetLayout = nullptr;
		CreateDescriptorSetLayout(newSetLayout, setBindings);

		DescriptorSetLayoutData descriptorSetLayoutData{};
		descriptorSetLayoutData.DescriptorSetLayout = newSetLayout;
		descriptorSetLayoutData.DescriptorSetLayoutBindingDatas = setBindingDatas;
		_descriptorSetLayoutDatas.push_back(descriptorSetLayoutData);
	}
}

void KtShader::CreateDescriptorSetLayout(VkDescriptorSetLayout& layout, const std::span<VkDescriptorSetLayoutBinding> layoutBindings)
{
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
	layoutInfo.pBindings = layoutBindings.data();

	VK_CHECK_THROW(
		vkCreateDescriptorSetLayout(Framework.GetContext().GetDevice(), &layoutInfo, nullptr, &layout),
		"failed to create descriptor set layout!"
	);
}

void KtShader::CreateDescriptorSets()
{
	for (auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		std::array<VkDescriptorSetLayout, KT_FRAMES_IN_FLIGHT> layouts{};
		layouts.fill(descriptorSetLayoutData.DescriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(layouts.size());
		allocInfo.pSetLayouts = layouts.data();

		VK_CHECK_THROW(
			vkAllocateDescriptorSets(Framework.GetContext().GetDevice(), &allocInfo, descriptorSetLayoutData.DescriptorSets.data()),
			"failed to allocate descriptor sets!"
		);
	}

	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		UpdateDescriptorSets(static_cast<uint32_t>(i));
	}
}

void KtShader::UpdateDescriptorSets(const uint32_t imageIndex)
{
	std::vector<VkWriteDescriptorSet> writeDescriptorSets;

	std::vector<VkDescriptorImageInfo> descriptorImageInfos;
	std::vector<VkDescriptorBufferInfo> descriptorBufferInfos;
	size_t descriptorImageCount = 0;
	size_t descriptorBufferCount = 0;
	for (const auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		for (const auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			switch (descriptorSetLayoutBindingData.DescriptorType)
			{
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
			{
				++descriptorImageCount;
				break;
			}
			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
			case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
			{
				++descriptorBufferCount;
				break;
			}
			default:
				break;
			}
		}
	}
	descriptorImageInfos.reserve(descriptorImageCount);
	descriptorBufferInfos.reserve(descriptorBufferCount);

	for (const auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		for (const auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			VkWriteDescriptorSet writeDescriptorSet{};
			writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDescriptorSet.dstSet = descriptorSetLayoutData.DescriptorSets[imageIndex];
			writeDescriptorSet.dstBinding = descriptorSetLayoutBindingData.Binding;
			writeDescriptorSet.dstArrayElement = 0;
			writeDescriptorSet.descriptorType = descriptorSetLayoutBindingData.DescriptorType;
			writeDescriptorSet.descriptorCount = descriptorSetLayoutBindingData.DescriptorCount;
			switch (descriptorSetLayoutBindingData.DescriptorType)
			{
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
			{
				VkDescriptorImageInfo imageInfo = descriptorSetLayoutBindingData.ImageTexture->GetDescriptorImageInfo();
				descriptorImageInfos.push_back(imageInfo);
				writeDescriptorSet.pImageInfo = &descriptorImageInfos.back();
				KT_DEBUG_LOG("imageInfo ptr: %p", (void*)&descriptorImageInfos.back());
				break;
			}
			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
			case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
			{
				VkDescriptorBufferInfo bufferInfo{};
				bufferInfo.buffer = descriptorSetLayoutBindingData.Buffers[imageIndex].Buffer;
				bufferInfo.offset = 0;
				bufferInfo.range = descriptorSetLayoutBindingData.MemberSize * descriptorSetLayoutBindingData.MemberCounts[imageIndex];
				descriptorBufferInfos.push_back(bufferInfo);
				writeDescriptorSet.pBufferInfo = &descriptorBufferInfos.back();
				KT_DEBUG_LOG("bufferInfo ptr: %p", (void*)&descriptorBufferInfos.back());
				break;
			}
			default:
				break;
			}
			writeDescriptorSets.push_back(writeDescriptorSet);
		}
	}

	KT_DEBUG_LOG("ImageInfos");
	for (const auto& imageInfo : descriptorImageInfos)
	{
		KT_DEBUG_LOG("  %p", (void*)&imageInfo);
	}
	KT_DEBUG_LOG("BufferInfos");
	for (const auto& bufferInfo : descriptorBufferInfos)
	{
		KT_DEBUG_LOG("  %p", (void*)&bufferInfo);
	}

	vkUpdateDescriptorSets(Framework.GetContext().GetDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
}

void KtShader::CreateDescriptorPool(const std::span<VkDescriptorPoolSize> poolSizes, const uint32_t setCount)
{
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT) * setCount;

	VK_CHECK_THROW(
		vkCreateDescriptorPool(Framework.GetContext().GetDevice(), &poolInfo, nullptr, &_descriptorPool),
		"failed to create descriptor pool!"
	);
}

void KtShader::CreateShaderModule(VkShaderModule& shaderModule, const std::span<uint8_t> code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VK_CHECK_THROW(
		vkCreateShaderModule(Framework.GetContext().GetDevice(), &createInfo, nullptr, &shaderModule),
		"failed to create shader module!"
	);
}

void KtShader::CreateGraphicsPipeline()
{
	std::vector<VkShaderModule> shaderModules;
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	nlohmann::json json;
	KtSerializer().ReadData(_path, json);
	for (const auto& shader : json["shaders"])
	{
		const auto path = Framework.GetPath().GetFrameworkPath() / R"(shaders)" / shader["path"];
		std::vector<uint8_t> shaderCode = KtFile(path).ReadBinary();

		VkShaderModule shaderModule;
		CreateShaderModule(shaderModule, shaderCode);
		shaderModules.push_back(shaderModule);

		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = shader["shaderStage"];
		shaderStageInfo.module = shaderModule;
		shaderStageInfo.pName = "main";

		shaderStages.push_back(shaderStageInfo);
	}
	
	auto& dataRasterizer = json["rasterizer"];
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = dataRasterizer["depthClampEnable"];
	rasterizer.rasterizerDiscardEnable = dataRasterizer["rasterizerDiscardEnable"];
	rasterizer.polygonMode = dataRasterizer["polygonMode"];
	rasterizer.lineWidth = dataRasterizer["lineWidth"]; // For rasterizer.polygonMode = VK_POLYGON_MODE_LINE; 
	// enable wideLines GPU feature for thicker
	rasterizer.cullMode = dataRasterizer["cullMode"]; 
	rasterizer.frontFace = dataRasterizer["frontFace"];
	rasterizer.depthBiasEnable = dataRasterizer["depthBiasEnable"];
	rasterizer.depthBiasConstantFactor = dataRasterizer["depthBiasConstantFactor"]; // Optional
	rasterizer.depthBiasClamp = dataRasterizer["depthBiasClamp"]; // Optional
	rasterizer.depthBiasSlopeFactor = dataRasterizer["depthBiasSlopeFactor"]; // Optional

	auto& dataMultisampling = json["multisampling"];
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = dataMultisampling["sampleShadingEnable"]; // enable sample shading in the pipeline
	multisampling.rasterizationSamples = Framework.GetContext().GetMSAASamples();
	multisampling.minSampleShading = dataMultisampling["minSampleShading"]; // min fraction for sample shading; closer to one is smoother
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = dataMultisampling["alphaToCoverageEnable"]; // Optional
	multisampling.alphaToOneEnable = dataMultisampling["alphaToOneEnable"]; // Optional

	auto& dataDepthStencil = json["depthStencil"];
	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = dataDepthStencil["depthTestEnable"];
	depthStencil.depthWriteEnable = dataDepthStencil["depthWriteEnable"];
	depthStencil.depthCompareOp = dataDepthStencil["depthCompareOp"];
	depthStencil.depthBoundsTestEnable = dataDepthStencil["depthBoundsTestEnable"];
	depthStencil.minDepthBounds = dataDepthStencil["minDepthBounds"]; // Optional
	depthStencil.maxDepthBounds = dataDepthStencil["maxDepthBounds"]; // Optional
	depthStencil.stencilTestEnable = dataDepthStencil["stencilTestEnable"];
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	auto& dataColorBlendAttachment = json["colorBlendAttachment"];
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = 0;
	for (const auto& component : dataColorBlendAttachment["colorWriteMask"])
	{
		colorBlendAttachment.colorWriteMask |= component; 
	}	
	colorBlendAttachment.blendEnable = dataColorBlendAttachment["blendEnable"];
	colorBlendAttachment.srcColorBlendFactor = dataColorBlendAttachment["srcColorBlendFactor"];
	colorBlendAttachment.dstColorBlendFactor = dataColorBlendAttachment["dstColorBlendFactor"];
	colorBlendAttachment.colorBlendOp = dataColorBlendAttachment["colorBlendOp"];
	colorBlendAttachment.srcAlphaBlendFactor = dataColorBlendAttachment["srcAlphaBlendFactor"];
	colorBlendAttachment.dstAlphaBlendFactor = dataColorBlendAttachment["dstAlphaBlendFactor"];
	colorBlendAttachment.alphaBlendOp = dataColorBlendAttachment["alphaBlendOp"];

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(_shaderLayout.VertexInputBindingDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = _shaderLayout.VertexInputBindingDescriptions.data();
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(_shaderLayout.VertexInputAttributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = _shaderLayout.VertexInputAttributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)Framework.GetRenderer().GetSwapChainExtent().width;
	viewport.height = (float)Framework.GetRenderer().GetSwapChainExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = Framework.GetRenderer().GetSwapChainExtent();

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	std::vector<VkDynamicState> dynamicStates =
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();


	std::vector<VkDescriptorSetLayout> setLayouts;
	for (const auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		setLayouts.push_back(descriptorSetLayoutData.DescriptorSetLayout);
	}

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());;
	pipelineLayoutInfo.pSetLayouts = setLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	VK_CHECK_THROW(
		vkCreatePipelineLayout(Framework.GetContext().GetDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout),
		"failed to create pipeline layout!"
	);

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = _pipelineLayout;
	pipelineInfo.renderPass = Framework.GetRenderer().GetRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	VK_CHECK_THROW(
		vkCreateGraphicsPipelines(Framework.GetContext().GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline),
		"failed to create graphics pipeline!"
	);

	for (auto shaderModule : shaderModules)
	{
		vkDestroyShaderModule(Framework.GetContext().GetDevice(), shaderModule, nullptr);
	}
}

void KtShader::CreateDescriptorSetLayoutBindingBuffers()
{
	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		CreateDescriptorSetLayoutBindingBuffers(static_cast<uint32_t>(i));
	}
}

void KtShader::CreateDescriptorSetLayoutBindingBuffers(const uint32_t imageIndex)
{
	for (auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		for (auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			CreateDescriptorSetLayoutBindingBuffer(descriptorSetLayoutBindingData, imageIndex);
		}
	}
}

void KtShader::CreateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData)
{
	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		CreateDescriptorSetLayoutBindingBuffer(descriptorSetLayoutBindingData, static_cast<uint32_t>(i));
	}
}

void KtShader::CreateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const uint32_t imageIndex)
{
	VkBufferUsageFlagBits bufferUsageFlagBits{};
	switch (descriptorSetLayoutBindingData.DescriptorType)
	{
	case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
	{
		bufferUsageFlagBits = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
		break;
	}
	case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
	{
		bufferUsageFlagBits = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
		break;
	}
	case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
	case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
	{
		bufferUsageFlagBits = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		break;
	}
	case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
	case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
	{
		bufferUsageFlagBits = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		break;
	}
	default:
		break;
	}

	if (bufferUsageFlagBits)
	{
		Framework.GetContext().CreateBuffer(
			descriptorSetLayoutBindingData.MemberSize * descriptorSetLayoutBindingData.MemberCounts[imageIndex],
			bufferUsageFlagBits | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			0,
			descriptorSetLayoutBindingData.Buffers[imageIndex],
			VMA_MEMORY_USAGE_GPU_ONLY
		);

		Framework.GetContext().CreateBuffer(
			descriptorSetLayoutBindingData.MemberSize * descriptorSetLayoutBindingData.MemberCounts[imageIndex],
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VMA_ALLOCATION_CREATE_MAPPED_BIT,
			descriptorSetLayoutBindingData.StagingBuffers[imageIndex],
			VMA_MEMORY_USAGE_CPU_TO_GPU
		);
	}
}

void KtShader::CreateDescriptorSetLayoutBindingImages()
{
	for (auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		for (auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			CreateDescriptorSetLayoutBindingImage(descriptorSetLayoutBindingData);
		}
	}
}

void KtShader::CreateDescriptorSetLayoutBindingImage(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData)
{
	if (descriptorSetLayoutBindingData.DescriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
	{
		const auto path = Framework.GetPath().GetSolutionPath() / R"(assets\models\viking_room.png)";
		auto* imageTexture = Framework.GetImageTextureManager().Get(path);
		if (imageTexture)
		{
			descriptorSetLayoutBindingData.ImageTexture = imageTexture;
		}
		else
		{
			descriptorSetLayoutBindingData.ImageTexture = Framework.GetImageTextureManager().Create(path);
		}
	}
}

void KtShader::DebugLogDescriptorSetLayoutData() const
{
	for (const auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		KT_DEBUG_LOG("DescriptorSetLayout: %p", (void*)descriptorSetLayoutData.DescriptorSetLayout);
		KT_DEBUG_LOG("| DescriptorSets");
		for (const auto& descriptorSet : descriptorSetLayoutData.DescriptorSets)
		{
			KT_DEBUG_LOG("| | %p", (void*)descriptorSet);
		}
		KT_DEBUG_LOG("| DescriptorSetLayoutBindingDatas");
		for (const auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			KT_DEBUG_LOG("| | Name: %s", descriptorSetLayoutBindingData.Name.c_str());
			KT_DEBUG_LOG("| | Binding: %u", descriptorSetLayoutBindingData.Binding);
			KT_DEBUG_LOG("| | DescriptorCount: %u", descriptorSetLayoutBindingData.DescriptorCount);
			KT_DEBUG_LOG("| | DescriptorType: %u", descriptorSetLayoutBindingData.DescriptorType);
			KT_DEBUG_LOG("| | MemberSize: %llu", descriptorSetLayoutBindingData.MemberSize);
			KT_DEBUG_LOG("| | StageFlags: %u", descriptorSetLayoutBindingData.StageFlags);
			KT_DEBUG_LOG("| | Buffers");
			for (const auto& buffer : descriptorSetLayoutBindingData.Buffers)
			{
				KT_DEBUG_LOG("| | | %p", (void*)(&buffer));
			}
			for (const auto& stagingBuffer : descriptorSetLayoutBindingData.StagingBuffers)
			{
				KT_DEBUG_LOG("| | | %p", (void*)(&stagingBuffer));
			}
		}
	}
}

void KtShader::UpdateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, void* data, const uint32_t imageIndex)
{
	const size_t dataSize = descriptorSetLayoutBindingData.MemberSize * descriptorSetLayoutBindingData.MemberCounts[imageIndex];

	memcpy(descriptorSetLayoutBindingData.StagingBuffers[imageIndex].AllocationInfo.pMappedData, data, dataSize);

	Framework.GetContext().CopyBuffer(
		descriptorSetLayoutBindingData.StagingBuffers[imageIndex].Buffer,
		descriptorSetLayoutBindingData.Buffers[imageIndex].Buffer,
		dataSize
	);
}

void KtShader::UpdateDescriptorSetLayoutBindingMemberCount(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const size_t memberCount, const uint32_t imageIndex)
{
	if (descriptorSetLayoutBindingData.MemberCounts[imageIndex] != memberCount)
	{
		KT_DEBUG_LOG("descriptorSetLayoutBindingData member count at frame %u: %llu", imageIndex, memberCount);
		descriptorSetLayoutBindingData.MemberCounts[imageIndex] = std::max(1llu, memberCount);

		vmaDestroyBuffer(Framework.GetContext().GetAllocator(), descriptorSetLayoutBindingData.Buffers[imageIndex].Buffer, descriptorSetLayoutBindingData.Buffers[imageIndex].Allocation);
		vmaDestroyBuffer(Framework.GetContext().GetAllocator(), descriptorSetLayoutBindingData.StagingBuffers[imageIndex].Buffer, descriptorSetLayoutBindingData.StagingBuffers[imageIndex].Allocation);

		CreateDescriptorSetLayoutBindingBuffer(descriptorSetLayoutBindingData, imageIndex);
		UpdateDescriptorSets(imageIndex);
	}
}

void KtShader::UpdateDescriptorSetLayoutBindingImage(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, KtImageTexture* imageTexture)
{
	descriptorSetLayoutBindingData.ImageTexture = imageTexture;
}

KtShader::DescriptorSetLayoutBindingData* KtShader::GetDescriptorSetLayoutBinding(const std::string_view name)
{
	for (auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		for (auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			if (descriptorSetLayoutBindingData.Name == name)
			{
				return &descriptorSetLayoutBindingData;
			}
		}
	}

	return nullptr;
}

void KtShader::CreateDescriptorPools()
{
	std::vector<VkDescriptorPoolSize> poolSizes{};
	for (const auto& [index, setLayout] : _shaderLayout.DescriptorSetLayouts)
	{
		for (const auto& binding : setLayout.DescriptorSetLayoutBindings)
		{
			VkDescriptorPoolSize poolSize{};
			poolSize.type = binding.DescriptorType;
			poolSize.descriptorCount = static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT);
			poolSizes.push_back(poolSize);
		}
	}
	CreateDescriptorPool(poolSizes, static_cast<uint32_t>(_shaderLayout.DescriptorSetLayouts.size()));
}

void KtShader::CreateShaderLayout()
{
	nlohmann::json json;
	KtSerializer().ReadData(_path, json);
	for (const auto& shader : json["shaders"])
	{
		const auto path = Framework.GetPath().GetFrameworkPath() / R"(shaders)" / shader["path"];
		std::vector<uint8_t> shaderCode = KtFile(path).ReadBinary();
		PopulateShaderLayout(shaderCode, shader["shaderStage"]);
	}
}

static const size_t GetTypeSize(const SpvReflectTypeDescription* type)
{
	if (!type)
	{
		KT_DEBUG_LOG("error: type is null");
		return 0;
	}

	if (type->type_flags & SPV_REFLECT_TYPE_FLAG_STRUCT)
	{
		size_t structSize = 0;
		for (uint32_t i = 0; i < type->member_count; ++i)
		{
			const SpvReflectTypeDescription* member = &type->members[i];
			structSize += GetTypeSize(member); // Sum up members
		}
		return structSize;  // Return immediately for structs
	}

	size_t size = 0;

	if (type->type_flags & SPV_REFLECT_TYPE_FLAG_BOOL)
	{
		size = 1;  // Booleans are typically 1 byte
	}
	else if (type->type_flags & SPV_REFLECT_TYPE_FLAG_INT)
	{
		size = type->traits.numeric.scalar.width / 8;  // Integer size in bytes
	}
	else if (type->type_flags & SPV_REFLECT_TYPE_FLAG_FLOAT)
	{
		size = type->traits.numeric.scalar.width / 8;  // Floating point size in bytes
	}
	if (type->type_flags & SPV_REFLECT_TYPE_FLAG_VECTOR)
	{
		size *= type->traits.numeric.vector.component_count;
	}
	if (type->type_flags & SPV_REFLECT_TYPE_FLAG_MATRIX)
	{
		size *= type->traits.numeric.matrix.column_count;
	}

	return size;
}

void KtShader::PopulateShaderLayout(const std::span<uint8_t> spirvData, const VkShaderStageFlagBits shaderStage)
{
	SpvReflectShaderModule module;
	SpvReflectResult result = spvReflectCreateShaderModule(spirvData.size() * sizeof(uint8_t), spirvData.data(), &module, 0);

	if (result != SPV_REFLECT_RESULT_SUCCESS)
	{
		KT_DEBUG_LOG("spvReflectCreateShaderModule() returned: %u", result);
		throw std::runtime_error("couldn't create spirv reflect shader module");
	}

	_shaderLayout.ShaderStages.push_back(shaderStage);

	// Descriptor sets
	uint32_t setCount = 0;
	spvReflectEnumerateDescriptorSets(&module, &setCount, nullptr);
	std::vector<SpvReflectDescriptorSet*> sets(setCount);
	spvReflectEnumerateDescriptorSets(&module, &setCount, sets.data());

	for (auto* set : sets)
	{
		KT_DEBUG_LOG("Descriptor Set: %d, Binding Count: %d", set->set, set->binding_count);
		for (uint32_t i = 0; i < set->binding_count; i++)
		{
			const SpvReflectDescriptorBinding* binding = set->bindings[i];	

			KtShaderLayout::DescriptorSetLayout::DescriptorSetLayoutBinding ktBinding{};
			ktBinding.Name = binding->name;
			ktBinding.Binding = binding->binding;
			ktBinding.DescriptorCount = binding->count;
			ktBinding.DescriptorType = static_cast<VkDescriptorType>(binding->descriptor_type);
			ktBinding.StageFlags = shaderStage;
			ktBinding.Size = GetTypeSize(binding->type_description);

			_shaderLayout.DescriptorSetLayouts[set->set].DescriptorSetLayoutBindings.push_back(ktBinding);
		}
	}

	// Push constants
	uint32_t pushConstantCount = 0;
	spvReflectEnumeratePushConstantBlocks(&module, &pushConstantCount, nullptr);
	std::vector<SpvReflectBlockVariable*> push_constants(pushConstantCount);
	spvReflectEnumeratePushConstantBlocks(&module, &pushConstantCount, push_constants.data());
	for (auto* push_constant : push_constants)
	{
		KT_DEBUG_LOG("Push Constant Block: Size %llu bytes", GetTypeSize(push_constant->type_description));
	}

	if (shaderStage == VK_SHADER_STAGE_VERTEX_BIT)
	{
		uint32_t inputCount = 0;
		spvReflectEnumerateInputVariables(&module, &inputCount, nullptr);
		std::vector<SpvReflectInterfaceVariable*> inputs(inputCount);
		spvReflectEnumerateInputVariables(&module, &inputCount, inputs.data());

		size_t offset = 0;
		for (auto* input : inputs)
		{
			KT_DEBUG_LOG("Vertex Input: Location %d, Format %d, Size %llu bytes, Name: %s", input->location, input->format, GetTypeSize(input->type_description), input->name);
			if (input->location != -1) // TODO: wtf it's an uint32_t
			{
				VkVertexInputAttributeDescription vertexInputAttributeDescription{};
				vertexInputAttributeDescription.location = input->location;
				vertexInputAttributeDescription.binding = 0; // TODO: from VkVertexInputBindingDescription ?
				vertexInputAttributeDescription.format = static_cast<VkFormat>(input->format);
				vertexInputAttributeDescription.offset = static_cast<uint32_t>(offset);
				_shaderLayout.VertexInputAttributeDescriptions.push_back(vertexInputAttributeDescription);

				offset += GetTypeSize(input->type_description);
			}
		}
		VkVertexInputBindingDescription vertexInputBindingDescription{};
		vertexInputBindingDescription.binding = 0;
		vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		vertexInputBindingDescription.stride = static_cast<uint32_t>(offset);
		_shaderLayout.VertexInputBindingDescriptions.push_back(vertexInputBindingDescription);
	}
	spvReflectDestroyShaderModule(&module);
}
