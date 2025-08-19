#include "Shader.h"
#include "Framework.h"
#include "vk_utils.h"
#include "Context.h"
#include "ImageTextureManager.h"
#include "ImageTexture.h"
#include "Path.h"
#include "File.h"
#include <spirv-reflect/spirv_reflect.h>
#include <nlohmann/json.hpp>
#include "Serializer.h"
#include "log.h"
#include "Renderer.h"

#define KT_LOG_IMPORTANCE_LEVEL_SHADER KT_LOG_IMPORTANCE_LEVEL_MEDIUM

static constexpr uint32_t MAX_BINDLESS_TEXTURES{ 8192 }; // todo: editable in project settings

static constexpr VkDescriptorBindingFlags BINDLESS_TEXTURE_FLAGS{
	VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
	VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT |
	VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT 
};

KtShader::KtShader(const std::filesystem::path& path) :
	path_(path)
{
}

void KtShader::Init()
{
	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "initializing shader '%s'", path_.string().c_str());
	CreateShaderLayout();
	CreateDescriptorSetLayouts();
	DebugLogDescriptorSetLayoutData();
	CreateDescriptorPools();
	CreateDescriptorSets();
	CreateDescriptorSetLayoutBindings();
	CreateGraphicsPipeline();
	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "initialized shader '%s'", path_.string().c_str());
}

void KtShader::Cleanup()
{
	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "cleaning up shader '%s'", _name.c_str());

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

	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "cleaned up shader '%s'", _name.c_str());
}

const std::filesystem::path& KtShader::GetPath() const
{
	return path_;
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
	std::vector<VkDescriptorSet> descriptorSets{};
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
		const size_t bindingCount = setLayout.DescriptorSetLayoutBindings.size();

		std::vector<VkDescriptorSetLayoutBinding> setBindings{};
		std::vector<VkDescriptorBindingFlags> setBindingFlags{};
		std::vector<DescriptorSetLayoutBindingData> setBindingDatas{};
		setBindings.reserve(bindingCount);
		setBindingFlags.reserve(bindingCount);
		setBindingDatas.reserve(bindingCount);
		for (const auto& ktBinding : setLayout.DescriptorSetLayoutBindings)
		{

			VkDescriptorSetLayoutBinding vkBinding{};
			vkBinding.binding = ktBinding.Binding;
			if (ktBinding.DescriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
			{
				vkBinding.descriptorCount = MAX_BINDLESS_TEXTURES;
				setBindingFlags.push_back(BINDLESS_TEXTURE_FLAGS);
			}
			else
			{
				vkBinding.descriptorCount = ktBinding.DescriptorCount;
				setBindingFlags.push_back(0);
			}
			vkBinding.descriptorType = ktBinding.DescriptorType;
			vkBinding.stageFlags = ktBinding.ShaderStageFlags;
			vkBinding.pImmutableSamplers = nullptr; // Optional
			setBindings.push_back(vkBinding);

			const DescriptorSetLayoutBindingData bindingData{
				.Name = ktBinding.Name,
				.MemberSize = ktBinding.Size,
				.DescriptorType = ktBinding.DescriptorType,
				.Binding = ktBinding.Binding,
				.DescriptorCount = ktBinding.DescriptorCount,
				.ShaderStageFlags = ktBinding.ShaderStageFlags,
				.BindingFlags = ktBinding.DescriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER 
					? BINDLESS_TEXTURE_FLAGS 
				    : 0,
			};
			//bindingData.MemberCounts.fill(1);
			setBindingDatas.push_back(bindingData);
		}
		VkDescriptorSetLayout newSetLayout{ nullptr };
		CreateDescriptorSetLayout(newSetLayout, setBindings, setBindingFlags);

		const DescriptorSetLayoutData descriptorSetLayoutData{ 
			.DescriptorSetLayout = newSetLayout,
			.DescriptorSetLayoutBindingDatas = setBindingDatas,
		};
		_descriptorSetLayoutDatas.push_back(descriptorSetLayoutData);
	}

	for (auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		for (auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			_descriptorSetLayoutBindingDataRegistry[descriptorSetLayoutBindingData.Name] = &descriptorSetLayoutBindingData;
		}
	}
}

void KtShader::CreateDescriptorSetLayout(
	VkDescriptorSetLayout& layout, 
	const std::span<VkDescriptorSetLayoutBinding> layoutBindings,
	const std::span<VkDescriptorBindingFlags> bindingFlags)
{
	VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo{};
	bindingFlagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
	bindingFlagsInfo.bindingCount = static_cast<uint32_t>(bindingFlags.size());
	bindingFlagsInfo.pBindingFlags = bindingFlags.data();

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
	layoutInfo.pBindings = layoutBindings.data();
	layoutInfo.pNext = &bindingFlagsInfo;
	layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;

	VK_CHECK_THROW(
		vkCreateDescriptorSetLayout(Framework.GetContext().GetDevice(), &layoutInfo, nullptr, &layout),
		"failed to create descriptor set layout!"
	);
}

void KtShader::CreateDescriptorSets()
{
	for (auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		KtFramesInFlightArray<uint32_t> variableDescriptorCounts{};
		variableDescriptorCounts.fill(MAX_BINDLESS_TEXTURES);

		VkDescriptorSetVariableDescriptorCountAllocateInfo countInfo{};
		countInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
		countInfo.descriptorSetCount = static_cast<uint32_t>(variableDescriptorCounts.size());
		countInfo.pDescriptorCounts = variableDescriptorCounts.data();

		KtFramesInFlightArray<VkDescriptorSetLayout> layouts{};
		layouts.fill(descriptorSetLayoutData.DescriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(layouts.size());
		allocInfo.pSetLayouts = layouts.data();
		for (const auto& binding : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			if (binding.BindingFlags == BINDLESS_TEXTURE_FLAGS)
			{
				allocInfo.pNext = &countInfo;
				break;
			}
		}

		VK_CHECK_THROW(
			vkAllocateDescriptorSets(Framework.GetContext().GetDevice(), &allocInfo, descriptorSetLayoutData.DescriptorSets.data()),
			"failed to allocate descriptor sets!"
		);

		for (auto& bindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			bindingData.DescriptorSetLayout = descriptorSetLayoutData.DescriptorSetLayout;
			bindingData.DescriptorSets = descriptorSetLayoutData.DescriptorSets;
		}
	}
}

void KtShader::CreateDescriptorSetLayoutBindings()
{
	for (auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		for (auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
			{
				UpdateDescriptorSetLayoutBindingBufferMemberCount(descriptorSetLayoutBindingData, 1, static_cast<uint32_t>(i));
				CreateDescriptorSetLayoutBindingImageSampler(descriptorSetLayoutBindingData, static_cast<uint32_t>(i));
			}
		}
	}
}

void KtShader::CreateDescriptorPool(const std::span<VkDescriptorPoolSize> poolSizes, const uint32_t setCount)
{
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.maxSets = static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT) * setCount;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

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

	nlohmann::json json{};
	KtSerializer serializer{};
	serializer.ReadData(path_, json);
	for (const auto& shader : json["shaders"])
	{
		const auto path = Framework.GetPath().GetFrameworkPath() / "shaders" / shader["path"];
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
	pipelineInfo.basePipelineHandle = nullptr; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	VK_CHECK_THROW(
		vkCreateGraphicsPipelines(Framework.GetContext().GetDevice(), nullptr, 1, &pipelineInfo, nullptr, &_graphicsPipeline),
		"failed to create graphics pipeline!"
	);

	for (auto shaderModule : shaderModules)
	{
		vkDestroyShaderModule(Framework.GetContext().GetDevice(), shaderModule, nullptr);
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

void KtShader::CreateDescriptorSetLayoutBindingImageSampler(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const uint32_t imageIndex)
{
	if (!GetIsImageSamplerDescriptorType(descriptorSetLayoutBindingData.DescriptorType))
	{
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "descriptor type '%u' isn't an image sampler type", descriptorSetLayoutBindingData.DescriptorType);
		return;
	}

	static const auto path = Framework.GetPath().GetSolutionPath() / R"(assets\textures\default_texture.jpg)";
	static const auto* imageTexture = Framework.GetImageTextureManager().Get(path);

	UpdateDescriptorSetLayoutBindingImageSampler(descriptorSetLayoutBindingData, { imageTexture->GetDescriptorImageInfo() }, imageIndex);
}

void KtShader::DebugLogDescriptorSetLayoutData() const
{
	for (const auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "DescriptorSetLayout: %p", (void*)descriptorSetLayoutData.DescriptorSetLayout);
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "| DescriptorSets");
		for (const auto& descriptorSet : descriptorSetLayoutData.DescriptorSets)
		{
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "| | %p", (void*)descriptorSet);
		}
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "| DescriptorSetLayoutBindingDatas");
		for (const auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "| | Name: %s", descriptorSetLayoutBindingData.Name.c_str());
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "| | Binding: %u", descriptorSetLayoutBindingData.Binding);
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "| | DescriptorCount: %u", descriptorSetLayoutBindingData.DescriptorCount);
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "| | DescriptorType: %u", descriptorSetLayoutBindingData.DescriptorType);
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "| | MemberSize: %llu", descriptorSetLayoutBindingData.MemberSize);
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "| | StageFlags: %u", descriptorSetLayoutBindingData.ShaderStageFlags);
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "| | Buffers");
			for (const auto& buffer : descriptorSetLayoutBindingData.Buffers)
			{
				KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "| | | %p", (void*)(&buffer));
			}
			for (const auto& stagingBuffer : descriptorSetLayoutBindingData.StagingBuffers)
			{
				KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "| | | %p", (void*)(&stagingBuffer));
			}
		}
	}
}

bool KtShader::GetIsBufferDescriptorType(const VkDescriptorType descriptorType) const
{
	return descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER ||
		descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER ||
		descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC ||
		descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC ||
		descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ||
		descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
}

bool KtShader::GetIsImageSamplerDescriptorType(const VkDescriptorType descriptorType) const
{
	return descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
}

void KtShader::UpdateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const void* data, const uint32_t imageIndex)
{
	const size_t dataSize = descriptorSetLayoutBindingData.MemberSize * descriptorSetLayoutBindingData.MemberCounts[imageIndex];

	memcpy(descriptorSetLayoutBindingData.StagingBuffers[imageIndex].AllocationInfo.pMappedData, data, dataSize);

	Framework.GetContext().CopyBuffer(
		descriptorSetLayoutBindingData.StagingBuffers[imageIndex].Buffer,
		descriptorSetLayoutBindingData.Buffers[imageIndex].Buffer,
		dataSize
	);
}

void KtShader::UpdateDescriptorSetLayoutBindingBufferMemberCount(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const size_t memberCount, const uint32_t imageIndex)
{
	if (!GetIsBufferDescriptorType(descriptorSetLayoutBindingData.DescriptorType))
	{
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "descriptor type '%u' isn't a buffer type", descriptorSetLayoutBindingData.DescriptorType);
		return;
	}

	if (descriptorSetLayoutBindingData.MemberCounts[imageIndex] == memberCount)
	{
		return;
	}

	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_LOW, "descriptorSetLayoutBindingData member count at frame %u: %llu", imageIndex, memberCount);
	descriptorSetLayoutBindingData.MemberCounts[imageIndex] = std::max(1llu, memberCount);

	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), descriptorSetLayoutBindingData.Buffers[imageIndex].Buffer, descriptorSetLayoutBindingData.Buffers[imageIndex].Allocation);
	vmaDestroyBuffer(Framework.GetContext().GetAllocator(), descriptorSetLayoutBindingData.StagingBuffers[imageIndex].Buffer, descriptorSetLayoutBindingData.StagingBuffers[imageIndex].Allocation);

	CreateDescriptorSetLayoutBindingBuffer(descriptorSetLayoutBindingData, imageIndex);

	UpdateDescriptorSetLayoutBindingBufferDescriptorSet(descriptorSetLayoutBindingData, imageIndex);
}

void KtShader::UpdateDescriptorSetLayoutBindingImageSampler(
	DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, 
	const std::vector<VkDescriptorImageInfo>& imageInfos, 
	const uint32_t imageIndex)
{
	if (!GetIsImageSamplerDescriptorType(descriptorSetLayoutBindingData.DescriptorType))
	{
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "descriptor type '%u' isn't an image sampler type", descriptorSetLayoutBindingData.DescriptorType);
		return;
	}

	const auto& currentImageInfos = descriptorSetLayoutBindingData.ImageInfos[imageIndex];

	bool isSame = currentImageInfos.size() == imageInfos.size();

	if (isSame)
	{
		for (size_t i = 0; i < imageInfos.size(); i++)
		{
			if (currentImageInfos[i].imageLayout != imageInfos[i].imageLayout ||
				currentImageInfos[i].imageView != imageInfos[i].imageView ||
				currentImageInfos[i].sampler != imageInfos[i].sampler)
			{
				isSame = false;
				break;
			}
		}
	}

	if (!isSame)
	{
		descriptorSetLayoutBindingData.DescriptorCount = static_cast<uint32_t>(imageInfos.size());
		descriptorSetLayoutBindingData.ImageInfos[imageIndex] = imageInfos;
		UpdateDescriptorSetLayoutBindingImageSamplerDescriptorSet(descriptorSetLayoutBindingData, imageIndex);
	}
}

void KtShader::UpdateDescriptorSetLayoutBindingBufferDescriptorSet(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const uint32_t imageIndex)
{
	VkWriteDescriptorSet writeDescriptorSet{};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet = descriptorSetLayoutBindingData.DescriptorSets[imageIndex];
	writeDescriptorSet.dstBinding = descriptorSetLayoutBindingData.Binding;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorType = descriptorSetLayoutBindingData.DescriptorType;
	writeDescriptorSet.descriptorCount = descriptorSetLayoutBindingData.DescriptorCount;

	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.buffer = descriptorSetLayoutBindingData.Buffers[imageIndex].Buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = descriptorSetLayoutBindingData.MemberSize * descriptorSetLayoutBindingData.MemberCounts[imageIndex];
	writeDescriptorSet.pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(Framework.GetContext().GetDevice(), 1, &writeDescriptorSet, 0, nullptr);
}

void KtShader::UpdateDescriptorSetLayoutBindingImageSamplerDescriptorSet(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const uint32_t imageIndex)
{
	VkWriteDescriptorSet writeDescriptorSet{};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet = descriptorSetLayoutBindingData.DescriptorSets[imageIndex];
	writeDescriptorSet.dstBinding = descriptorSetLayoutBindingData.Binding;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorType = descriptorSetLayoutBindingData.DescriptorType;
	writeDescriptorSet.descriptorCount = descriptorSetLayoutBindingData.DescriptorCount;
	writeDescriptorSet.pImageInfo = descriptorSetLayoutBindingData.ImageInfos[imageIndex].data();

	vkUpdateDescriptorSets(Framework.GetContext().GetDevice(), 1, &writeDescriptorSet, 0, nullptr);
}

KtShader::DescriptorSetLayoutBindingData* KtShader::GetDescriptorSetLayoutBinding(const std::string& name) 
{
	const auto it = _descriptorSetLayoutBindingDataRegistry.find(name);
	if (it != _descriptorSetLayoutBindingDataRegistry.end())
	{
		return it->second;
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
	nlohmann::json json{};
	KtSerializer serializer{};
	serializer.ReadData(path_, json);
	for (const auto& shader : json["shaders"])
	{
		const auto path = Framework.GetPath().GetFrameworkPath() / "shaders" / shader["path"];
		std::vector<uint8_t> shaderCode = KtFile(path).ReadBinary();
		PopulateShaderLayout(shaderCode, shader["shaderStage"]);
	}
}

static const size_t GetTypeSize(const SpvReflectTypeDescription* type)
{
	if (!type)
	{
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "error: type is null");
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
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "spvReflectCreateShaderModule() returned: %u", result);
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
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Descriptor Set: %d, Binding Count: %d", set->set, set->binding_count);
		for (uint32_t i = 0; i < set->binding_count; i++)
		{
			const SpvReflectDescriptorBinding* binding = set->bindings[i];	

			KtShaderLayout::DescriptorSetLayout::DescriptorSetLayoutBinding ktBinding{};
			ktBinding.Name = binding->name;
			ktBinding.Binding = binding->binding;
			ktBinding.DescriptorCount = binding->count;
			ktBinding.DescriptorType = static_cast<VkDescriptorType>(binding->descriptor_type);
			ktBinding.ShaderStageFlags = shaderStage;
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
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Push Constant Block: Size %llu bytes", GetTypeSize(push_constant->type_description));
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
			KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Vertex Input: Location %d, Format %d, Size %llu bytes, Name: %s", input->location, input->format, GetTypeSize(input->type_description), input->name);
			if (input->location != UINT32_MAX)
			{
				VkVertexInputAttributeDescription vertexInputAttributeDescription{};
				vertexInputAttributeDescription.location = input->location;
				vertexInputAttributeDescription.binding = 0;
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
