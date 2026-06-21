#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_io/File.h>
#include <kotono_io/Serializer.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>
#include <nlohmann/json.hpp>
#include <spirv-reflect/spirv_reflect.h>

#define KT_LOG_IMPORTANCE_LEVEL_SHADER ELogImportanceLevel::Medium

static constexpr u32 MAX_BINDLESS_TEXTURES{ 8192 }; // todo: editable in project settings

static constexpr VkDescriptorBindingFlags BINDLESS_TEXTURE_FLAGS{
	VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
	VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT |
	VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT
};

KtShader::KtShader(const UPath& path) 
	: path_(path)
{
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "initializing shader {}", path_.ToString());
	CreateShaderLayout();
	CreateDescriptorSetLayouts();
	DebugLogDescriptorSetLayoutData();
	CreateDescriptorPools();
	CreateDescriptorSets();
	CreateDescriptorSetLayoutBindings();
	CreateGraphicsPipeline();
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "initialized shader {}", path_.ToString());
}

KtShader::~KtShader()
{
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "cleaning up shader {}", path_.ToString());

	vkDestroyPipeline(Context.GetDevice(), graphicsPipeline_, nullptr);
	vkDestroyPipelineLayout(Context.GetDevice(), pipelineLayout_, nullptr);

	for (const auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		for (const auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			for (size i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
			{
				vmaDestroyBuffer(Context.GetAllocator(), descriptorSetLayoutBindingData.Buffers[i].Buffer, descriptorSetLayoutBindingData.Buffers[i].Allocation);
				vmaDestroyBuffer(Context.GetAllocator(), descriptorSetLayoutBindingData.StagingBuffers[i].Buffer, descriptorSetLayoutBindingData.StagingBuffers[i].Allocation);
			}
		}

		vkDestroyDescriptorSetLayout(Context.GetDevice(), descriptorSetLayoutData.DescriptorSetLayout, nullptr);
	}

	vkDestroyDescriptorPool(Context.GetDevice(), descriptorPool_, nullptr);

	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "cleaned up shader {}", path_.ToString());
}

const UPath& KtShader::Path() const
{
	return path_;
}

VkPipeline KtShader::GetGraphicsPipeline() const
{
	return graphicsPipeline_;
}

VkPipelineLayout KtShader::GetPipelineLayout() const
{
	return pipelineLayout_;
}

void KtShader::CmdBind(VkCommandBuffer commandBuffer) const
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline_);
}

void KtShader::CmdBindDescriptorSets(VkCommandBuffer commandBuffer, const u32 imageIndex) const
{
	std::vector<VkDescriptorSet> descriptorSets{};
	descriptorSets.reserve(descriptorSetLayoutDatas_.size());
	for (const auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		descriptorSets.push_back(descriptorSetLayoutData.DescriptorSets[imageIndex]);
	}
	vkCmdBindDescriptorSets(
		commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout_,
		0, static_cast<u32>(descriptorSets.size()), descriptorSets.data(), 0, nullptr
	);
}

void KtShader::CreateDescriptorSetLayouts()
{
	descriptorSetLayoutDatas_.reserve(shaderLayout_.DescriptorSetLayouts.size());
	for (const auto& [set, setLayout] : shaderLayout_.DescriptorSetLayouts)
	{
		const size bindingCount = setLayout.Bindings.size();

		std::vector<VkDescriptorSetLayoutBinding> setBindings{};
		std::vector<VkDescriptorBindingFlags> setBindingFlags{};
		std::vector<DescriptorSetLayoutBindingData> setBindingDatas{};
		setBindings.reserve(bindingCount);
		setBindingFlags.reserve(bindingCount);
		setBindingDatas.reserve(bindingCount);
		for (const auto& ktBinding : setLayout.Bindings)
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
		descriptorSetLayoutDatas_.push_back(descriptorSetLayoutData);
	}

	for (auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		for (auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			descriptorSetLayoutBindingDataRegistry_[descriptorSetLayoutBindingData.Name] = &descriptorSetLayoutBindingData;
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
	bindingFlagsInfo.bindingCount = static_cast<u32>(bindingFlags.size());
	bindingFlagsInfo.pBindingFlags = bindingFlags.data();

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<u32>(layoutBindings.size());
	layoutInfo.pBindings = layoutBindings.data();
	layoutInfo.pNext = &bindingFlagsInfo;
	layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;

	VK_CHECK_THROW(
		vkCreateDescriptorSetLayout(Context.GetDevice(), &layoutInfo, nullptr, &layout),
		"failed to create descriptor set layout!"
	);
}

void KtShader::CreateDescriptorSets()
{
	for (auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		KtFramesInFlightArray<u32> variableDescriptorCounts{};
		variableDescriptorCounts.fill(MAX_BINDLESS_TEXTURES);

		VkDescriptorSetVariableDescriptorCountAllocateInfo countInfo{};
		countInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
		countInfo.descriptorSetCount = static_cast<u32>(variableDescriptorCounts.size());
		countInfo.pDescriptorCounts = variableDescriptorCounts.data();

		KtFramesInFlightArray<VkDescriptorSetLayout> layouts{};
		layouts.fill(descriptorSetLayoutData.DescriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool_;
		allocInfo.descriptorSetCount = static_cast<u32>(layouts.size());
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
			vkAllocateDescriptorSets(Context.GetDevice(), &allocInfo, descriptorSetLayoutData.DescriptorSets.data()),
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
	for (auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		for (auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			for (size i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
			{
				UpdateDescriptorSetLayoutBindingBufferMemberCount(descriptorSetLayoutBindingData, 1, static_cast<u32>(i));
				CreateDescriptorSetLayoutBindingImageSampler(descriptorSetLayoutBindingData, static_cast<u32>(i));
			}
		}
	}
}

void KtShader::CreateDescriptorPool(const std::span<VkDescriptorPoolSize> poolSizes, const u32 setCount)
{
	const VkDescriptorPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT,
		.maxSets = static_cast<u32>(KT_FRAMES_IN_FLIGHT) * setCount,
		.poolSizeCount = static_cast<u32>(poolSizes.size()),
		.pPoolSizes = poolSizes.data(),
	};
	VK_CHECK_THROW(
		vkCreateDescriptorPool(Context.GetDevice(), &poolInfo, nullptr, &descriptorPool_),
		"failed to create descriptor pool!"
	);
}

void KtShader::CreateShaderModule(VkShaderModule& shaderModule, const std::span<u8> code)
{
	const VkShaderModuleCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size(),
		.pCode = reinterpret_cast<const u32*>(code.data()),
	};
	VK_CHECK_THROW(
		vkCreateShaderModule(Context.GetDevice(), &createInfo, nullptr, &shaderModule),
		"failed to create shader module!"
	);
}

void KtShader::CreateGraphicsPipeline()
{
	std::vector<VkShaderModule> shaderModules;
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	nlohmann::json json{};
	USerializer::Deserialize(json, path_);
	for (const auto& shader : json["shaders"])
	{
		const auto path{ UPath("${ENGINE_DIRECTORY}/Graphics/shaders").ToPath() / shader["path"] };
		std::vector shaderCode{ UFile(path).ReadBinary() };

		VkShaderModule shaderModule;
		CreateShaderModule(shaderModule, shaderCode);
		shaderModules.push_back(shaderModule);

		const VkPipelineShaderStageCreateInfo shaderStageInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = shader["shaderStage"],
			.module = shaderModule,
			.pName = "main",
		};
		shaderStages.push_back(shaderStageInfo);
	}

	const auto& dataRasterizer{ json["rasterizer"] };
	const VkPipelineRasterizationStateCreateInfo rasterizer{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = dataRasterizer["depthClampEnable"],
		.rasterizerDiscardEnable = dataRasterizer["rasterizerDiscardEnable"],
		.polygonMode = dataRasterizer["polygonMode"],
		.cullMode = dataRasterizer["cullMode"],
		.frontFace = dataRasterizer["frontFace"],
		.depthBiasEnable = dataRasterizer["depthBiasEnable"],
		.depthBiasConstantFactor = dataRasterizer["depthBiasConstantFactor"], // Optional
		.depthBiasClamp = dataRasterizer["depthBiasClamp"], // Optional
		.depthBiasSlopeFactor = dataRasterizer["depthBiasSlopeFactor"], // Optional
		.lineWidth = dataRasterizer["lineWidth"], // For rasterizer.polygonMode = VK_POLYGON_MODE_LINE, enable wideLines GPU feature for thicker
	};

	const auto& dataMultisampling{ json["multisampling"] };
	const VkPipelineMultisampleStateCreateInfo multisampling{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = Context.GetMSAASamples(),
		.sampleShadingEnable = dataMultisampling["sampleShadingEnable"], // enable sample shading in the pipeline
		.minSampleShading = dataMultisampling["minSampleShading"], // min fraction for sample shading, closer to one is smoother
		.pSampleMask = nullptr, // Optional
		.alphaToCoverageEnable = dataMultisampling["alphaToCoverageEnable"], // Optional
		.alphaToOneEnable = dataMultisampling["alphaToOneEnable"], // Optional
	};

	const auto& dataDepthStencil{ json["depthStencil"] };
	const VkPipelineDepthStencilStateCreateInfo depthStencil{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable = dataDepthStencil["depthTestEnable"],
		.depthWriteEnable = dataDepthStencil["depthWriteEnable"],
		.depthCompareOp = dataDepthStencil["depthCompareOp"],
		.depthBoundsTestEnable = dataDepthStencil["depthBoundsTestEnable"],
		.stencilTestEnable = dataDepthStencil["stencilTestEnable"],
		.front = {}, // Optional
		.back = {}, // Optional
		.minDepthBounds = dataDepthStencil["minDepthBounds"], // Optional
		.maxDepthBounds = dataDepthStencil["maxDepthBounds"], // Optional
	};

	const auto& dataColorBlendAttachment{ json["colorBlendAttachment"] };
	VkColorComponentFlags colorWriteMask{ 0 };
	for (const auto& component : dataColorBlendAttachment["colorWriteMask"])
	{
		colorWriteMask |= component;
	}
	const VkPipelineColorBlendAttachmentState colorBlendAttachment{
		.blendEnable = dataColorBlendAttachment["blendEnable"],
		.srcColorBlendFactor = dataColorBlendAttachment["srcColorBlendFactor"],
		.dstColorBlendFactor = dataColorBlendAttachment["dstColorBlendFactor"],
		.colorBlendOp = dataColorBlendAttachment["colorBlendOp"],
		.srcAlphaBlendFactor = dataColorBlendAttachment["srcAlphaBlendFactor"],
		.dstAlphaBlendFactor = dataColorBlendAttachment["dstAlphaBlendFactor"],
		.alphaBlendOp = dataColorBlendAttachment["alphaBlendOp"],
		.colorWriteMask = colorWriteMask,
	};

	const VkPipelineVertexInputStateCreateInfo vertexInputInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = static_cast<u32>(shaderLayout_.VertexInputBindingDescriptions.size()),
		.pVertexBindingDescriptions = shaderLayout_.VertexInputBindingDescriptions.data(),
		.vertexAttributeDescriptionCount = static_cast<u32>(shaderLayout_.VertexInputAttributeDescriptions.size()),
		.pVertexAttributeDescriptions = shaderLayout_.VertexInputAttributeDescriptions.data(),
	};

	const VkPipelineInputAssemblyStateCreateInfo inputAssembly{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE,
	};

	const VkViewport viewport{
		.x = 0.0f,
		.y = 0.0f,
		.width = (float)Renderer.SwapChainExtent().width,
		.height = (float)Renderer.SwapChainExtent().height,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};

	const VkRect2D scissor{
		.offset = { 0, 0 },
		.extent = Renderer.SwapChainExtent(),
	};

	const VkPipelineViewportStateCreateInfo viewportState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor,
	};

	const VkPipelineColorBlendStateCreateInfo colorBlending{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY, // Optional
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachment,
		.blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f }, // Optional
	};

	std::vector<VkDynamicState> dynamicStates =
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	const VkPipelineDynamicStateCreateInfo dynamicState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = static_cast<u32>(dynamicStates.size()),
		.pDynamicStates = dynamicStates.data(),
	};

	std::vector<VkDescriptorSetLayout> setLayouts;
	for (const auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		setLayouts.push_back(descriptorSetLayoutData.DescriptorSetLayout);
	}

	const VkPipelineLayoutCreateInfo pipelineLayoutInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = static_cast<u32>(setLayouts.size()),
		.pSetLayouts = setLayouts.data(),
		.pushConstantRangeCount = 0, // Optional
		.pPushConstantRanges = nullptr, // Optional
	};
	VK_CHECK_THROW(
		vkCreatePipelineLayout(Context.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout_),
		"failed to create pipeline layout!"
	);

	const VkGraphicsPipelineCreateInfo pipelineInfo{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = static_cast<u32>(shaderStages.size()),
		.pStages = shaderStages.data(),
		.pVertexInputState = &vertexInputInfo,
		.pInputAssemblyState = &inputAssembly,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizer,
		.pMultisampleState = &multisampling,
		.pDepthStencilState = &depthStencil,
		.pColorBlendState = &colorBlending,
		.pDynamicState = &dynamicState,
		.layout = pipelineLayout_,
		.renderPass = Renderer.RenderPass(),
		.subpass = 0,
		.basePipelineHandle = nullptr, // Optional
		.basePipelineIndex = -1, // Optional
	};
	VK_CHECK_THROW(
		vkCreateGraphicsPipelines(Context.GetDevice(), nullptr, 1, &pipelineInfo, nullptr, &graphicsPipeline_),
		"failed to create graphics pipeline!"
	);

	for (auto shaderModule : shaderModules)
	{
		vkDestroyShaderModule(Context.GetDevice(), shaderModule, nullptr);
	}
}

void KtShader::CreateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex)
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
		Context.CreateBuffer(
			descriptorSetLayoutBindingData.MemberSize * descriptorSetLayoutBindingData.MemberCounts[imageIndex],
			bufferUsageFlagBits | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			0,
			descriptorSetLayoutBindingData.Buffers[imageIndex],
			VMA_MEMORY_USAGE_GPU_ONLY
		);

		Context.CreateBuffer(
			descriptorSetLayoutBindingData.MemberSize * descriptorSetLayoutBindingData.MemberCounts[imageIndex],
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VMA_ALLOCATION_CREATE_MAPPED_BIT,
			descriptorSetLayoutBindingData.StagingBuffers[imageIndex],
			VMA_MEMORY_USAGE_CPU_TO_GPU
		);
	}
}

void KtShader::CreateDescriptorSetLayoutBindingImageSampler(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex)
{
	if (!GetIsImageSamplerDescriptorType(descriptorSetLayoutBindingData.DescriptorType))
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "descriptor type {} isn't an image sampler type", (u32)descriptorSetLayoutBindingData.DescriptorType);
		return;
	}

	static const UAsset texture{ UAssetManager<KtTexture>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/viking_room.png") };

	UpdateDescriptorSetLayoutBindingImageSampler(descriptorSetLayoutBindingData, { texture->GetDescriptorImageInfo() }, imageIndex);
}

void KtShader::DebugLogDescriptorSetLayoutData() const
{
	for (const auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "DescriptorSetLayout: {}", static_cast<void*>(descriptorSetLayoutData.DescriptorSetLayout));
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| DescriptorSets");
		for (const auto& descriptorSet : descriptorSetLayoutData.DescriptorSets)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | {}", static_cast<void*>(descriptorSet));
		}
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| DescriptorSetLayoutBindingDatas");
		for (const auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.DescriptorSetLayoutBindingDatas)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | Name: {}", descriptorSetLayoutBindingData.Name);
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | Binding: {}", descriptorSetLayoutBindingData.Binding);
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | DescriptorCount: {}", descriptorSetLayoutBindingData.DescriptorCount);
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | DescriptorType: {}", (u32)descriptorSetLayoutBindingData.DescriptorType);
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | MemberSize: {}", descriptorSetLayoutBindingData.MemberSize);
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | StageFlags: {}", descriptorSetLayoutBindingData.ShaderStageFlags);
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | Buffers");
			for (const auto& buffer : descriptorSetLayoutBindingData.Buffers)
			{
				KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics.DebugLogDescriptorSetLayoutData()", "| | | {}", (void*)(&buffer));
			}
			for (const auto& stagingBuffer : descriptorSetLayoutBindingData.StagingBuffers)
			{
				KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics.DebugLogDescriptorSetLayoutData()", "| | | {}", (void*)(&stagingBuffer));
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

void KtShader::UpdateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const void* data, const u32 imageIndex)
{
	const size dataSize{ descriptorSetLayoutBindingData.MemberSize * descriptorSetLayoutBindingData.MemberCounts[imageIndex] };

	memcpy(descriptorSetLayoutBindingData.StagingBuffers[imageIndex].AllocationInfo.pMappedData, data, dataSize);

	Context.CopyBuffer(
		descriptorSetLayoutBindingData.StagingBuffers[imageIndex].Buffer,
		descriptorSetLayoutBindingData.Buffers[imageIndex].Buffer,
		dataSize
	);
}

void KtShader::UpdateDescriptorSetLayoutBindingBufferMemberCount(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const size memberCount, const u32 imageIndex)
{
	if (!GetIsBufferDescriptorType(descriptorSetLayoutBindingData.DescriptorType))
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "descriptor type {} isn't a buffer type", (u32)descriptorSetLayoutBindingData.DescriptorType);
		return;
	}

	if (descriptorSetLayoutBindingData.MemberCounts[imageIndex] == memberCount)
	{
		return;
	}

	KT_LOG(ELogImportanceLevel::Low, "Graphics", "descriptorSetLayoutBindingData member count at frame {}: {}", imageIndex, memberCount);
	descriptorSetLayoutBindingData.MemberCounts[imageIndex] = std::max(1llu, memberCount);

	vmaDestroyBuffer(Context.GetAllocator(), descriptorSetLayoutBindingData.Buffers[imageIndex].Buffer, descriptorSetLayoutBindingData.Buffers[imageIndex].Allocation);
	vmaDestroyBuffer(Context.GetAllocator(), descriptorSetLayoutBindingData.StagingBuffers[imageIndex].Buffer, descriptorSetLayoutBindingData.StagingBuffers[imageIndex].Allocation);

	CreateDescriptorSetLayoutBindingBuffer(descriptorSetLayoutBindingData, imageIndex);

	UpdateDescriptorSetLayoutBindingBufferDescriptorSet(descriptorSetLayoutBindingData, imageIndex);
}

void KtShader::UpdateDescriptorSetLayoutBindingImageSampler(
	DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData,
	const std::vector<VkDescriptorImageInfo>& imageInfos,
	const u32 imageIndex)
{
	if (!GetIsImageSamplerDescriptorType(descriptorSetLayoutBindingData.DescriptorType))
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "descriptor type {} isn't an image sampler type", (u32)descriptorSetLayoutBindingData.DescriptorType);
		return;
	}

	const auto& currentImageInfos{ descriptorSetLayoutBindingData.ImageInfos[imageIndex] };

	bool isSame{ currentImageInfos.size() == imageInfos.size() };

	if (isSame)
	{
		for (size i{ 0 }; i < imageInfos.size(); ++i)
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
		descriptorSetLayoutBindingData.DescriptorCount = static_cast<u32>(imageInfos.size());
		descriptorSetLayoutBindingData.ImageInfos[imageIndex] = imageInfos;
		UpdateDescriptorSetLayoutBindingImageSamplerDescriptorSet(descriptorSetLayoutBindingData, imageIndex);
	}
}

void KtShader::UpdateDescriptorSetLayoutBindingBufferDescriptorSet(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex)
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

	vkUpdateDescriptorSets(Context.GetDevice(), 1, &writeDescriptorSet, 0, nullptr);
}

void KtShader::UpdateDescriptorSetLayoutBindingImageSamplerDescriptorSet(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex)
{
	VkWriteDescriptorSet writeDescriptorSet{};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet = descriptorSetLayoutBindingData.DescriptorSets[imageIndex];
	writeDescriptorSet.dstBinding = descriptorSetLayoutBindingData.Binding;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorType = descriptorSetLayoutBindingData.DescriptorType;
	writeDescriptorSet.descriptorCount = descriptorSetLayoutBindingData.DescriptorCount;
	writeDescriptorSet.pImageInfo = descriptorSetLayoutBindingData.ImageInfos[imageIndex].data();

	vkUpdateDescriptorSets(Context.GetDevice(), 1, &writeDescriptorSet, 0, nullptr);
}

KtShader::DescriptorSetLayoutBindingData* KtShader::GetDescriptorSetLayoutBinding(const std::string& name)
{
	const auto it = descriptorSetLayoutBindingDataRegistry_.find(name);
	if (it != descriptorSetLayoutBindingDataRegistry_.end())
	{
		return it->second;
	}
	return nullptr;
}

void KtShader::CreateDescriptorPools()
{
	std::vector<VkDescriptorPoolSize> poolSizes{};
	for (const auto& [index, setLayout] : shaderLayout_.DescriptorSetLayouts)
	{
		for (const auto& binding : setLayout.Bindings)
		{
			VkDescriptorPoolSize poolSize{};
			poolSize.type = binding.DescriptorType;
			poolSize.descriptorCount = static_cast<u32>(KT_FRAMES_IN_FLIGHT);
			poolSizes.push_back(poolSize);
		}
	}
	CreateDescriptorPool(poolSizes, static_cast<u32>(shaderLayout_.DescriptorSetLayouts.size()));
}

void KtShader::CreateShaderLayout()
{
	nlohmann::json json{};
	USerializer::Deserialize(json, path_);
	for (const auto& shader : json["shaders"])
	{
		const auto path{ UPath("${ENGINE_DIRECTORY}/Graphics/shaders").ToPath() / shader["path"] };
		std::vector<u8> shaderCode{ UFile(path).ReadBinary() };
		PopulateShaderLayout(shaderCode, shader["shaderStage"]);
	}
}

static const size GetTypeSize(const SpvReflectTypeDescription* type)
{
	if (!type)
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "type is null");
		return 0;
	}

	if (type->type_flags & SPV_REFLECT_TYPE_FLAG_STRUCT)
	{
		size structSize{ 0 };
		for (u32 i{ 0 }; i < type->member_count; ++i)
		{
			const SpvReflectTypeDescription* member{ &type->members[i] };
			structSize += GetTypeSize(member); // Sum up members
		}
		return structSize;  // Return immediately for structs
	}

	size size{ 0 };

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

void KtShader::PopulateShaderLayout(const std::span<u8> spirvData, const VkShaderStageFlagBits shaderStage)
{
	SpvReflectShaderModule module;
	SpvReflectResult result = spvReflectCreateShaderModule(spirvData.size() * sizeof(u8), spirvData.data(), &module);

	if (result != SPV_REFLECT_RESULT_SUCCESS)
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "spvReflectCreateShaderModule() returned: {}", (u32)result);
		throw std::runtime_error("couldn't create spirv reflect shader module");
	}

	shaderLayout_.ShaderStages.push_back(shaderStage);

	// Descriptor sets
	u32 setCount = 0;
	spvReflectEnumerateDescriptorSets(&module, &setCount, nullptr);
	std::vector<SpvReflectDescriptorSet*> sets(setCount);
	spvReflectEnumerateDescriptorSets(&module, &setCount, sets.data());

	for (auto* set : sets)
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "Descriptor Set: {}, Binding Count: {}", set->set, set->binding_count);
		for (u32 i = 0; i < set->binding_count; i++)
		{
			const SpvReflectDescriptorBinding* binding = set->bindings[i];

			KtShaderLayout::DescriptorSetLayout::Binding ktBinding{};
			ktBinding.Name = binding->name;
			ktBinding.Binding = binding->binding;
			ktBinding.DescriptorCount = binding->count;
			ktBinding.DescriptorType = static_cast<VkDescriptorType>(binding->descriptor_type);
			ktBinding.ShaderStageFlags = shaderStage;
			ktBinding.Size = GetTypeSize(binding->type_description);

			shaderLayout_.DescriptorSetLayouts[set->set].Bindings.push_back(ktBinding);
		}
	}

	// Push constants
	u32 pushConstantCount = 0;
	spvReflectEnumeratePushConstantBlocks(&module, &pushConstantCount, nullptr);
	std::vector<SpvReflectBlockVariable*> push_constants(pushConstantCount);
	spvReflectEnumeratePushConstantBlocks(&module, &pushConstantCount, push_constants.data());
	for (auto* push_constant : push_constants)
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "Push Constant Block: Size {} bytes", GetTypeSize(push_constant->type_description));
	}

	if (shaderStage == VK_SHADER_STAGE_VERTEX_BIT)
	{
		u32 inputCount = 0;
		spvReflectEnumerateInputVariables(&module, &inputCount, nullptr);
		std::vector<SpvReflectInterfaceVariable*> inputs(inputCount);
		spvReflectEnumerateInputVariables(&module, &inputCount, inputs.data());

		size offset = 0;
		for (auto* input : inputs)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "Vertex Input: Location {}, Format {}, Size {} bytes, Name: {}", input->location, (u32)input->format, GetTypeSize(input->type_description), input->name);
			if (input->location != UINT32_MAX)
			{
				VkVertexInputAttributeDescription vertexInputAttributeDescription{};
				vertexInputAttributeDescription.location = input->location;
				vertexInputAttributeDescription.binding = 0;
				vertexInputAttributeDescription.format = static_cast<VkFormat>(input->format);
				vertexInputAttributeDescription.offset = static_cast<u32>(offset);
				shaderLayout_.VertexInputAttributeDescriptions.push_back(vertexInputAttributeDescription);

				offset += GetTypeSize(input->type_description);
			}
		}
		VkVertexInputBindingDescription vertexInputBindingDescription{};
		vertexInputBindingDescription.binding = 0;
		vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		vertexInputBindingDescription.stride = static_cast<u32>(offset);
		shaderLayout_.VertexInputBindingDescriptions.push_back(vertexInputBindingDescription);
	}
	spvReflectDestroyShaderModule(&module);
}
