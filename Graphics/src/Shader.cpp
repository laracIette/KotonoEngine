#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_graphics/PipelineResourceManager.h>
#include <kotono_io/File.h>
#include <kotono_io/Serializer.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>
#include <nlohmann/json.hpp>
#include <spirv-reflect/spirv_reflect.h>

#define KT_LOG_IMPORTANCE_LEVEL_SHADER ELogImportanceLevel::High

static constexpr u32 MAX_BINDLESS_TEXTURES{ 8192 };

static constexpr VkDescriptorBindingFlags BINDLESS_TEXTURE_FLAGS{
VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
	VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT
};

UShader::UShader(const UPath& path) 
	: path_{ path }
{
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "initializing shader {0}", path_.ToString());
	//CreateShaderLayout();
	//CreateDescriptorSetLayouts();
	//DebugLogDescriptorSetLayoutData();
	//CreateDescriptorPools();
	//CreateDescriptorSets();
	//CreateDescriptorSetLayoutBindings();
	CreateGraphicsPipeline();
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "initialized shader {0}", path_.ToString());
}

UShader::~UShader()
{
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "cleaning up shader {0}", path_.ToString());

	vkDestroyPipeline(Context.GetDevice(), graphicsPipeline_, nullptr);
	//vkDestroyPipelineLayout(Context.GetDevice(), pipelineLayout_, nullptr);

	for (const auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		for (const auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.descriptorSetLayoutBindingDatas)
		{
			for (size i{ 0 }; i < KT_FRAMES_IN_FLIGHT; i++)
			{
				vmaDestroyBuffer(Context.GetAllocator(), descriptorSetLayoutBindingData.buffers[i].Buffer, descriptorSetLayoutBindingData.buffers[i].Allocation);
				vmaDestroyBuffer(Context.GetAllocator(), descriptorSetLayoutBindingData.stagingBuffers[i].Buffer, descriptorSetLayoutBindingData.stagingBuffers[i].Allocation);
			}
		}

		vkDestroyDescriptorSetLayout(Context.GetDevice(), descriptorSetLayoutData.descriptorSetLayout, nullptr);
	}

	vkDestroyDescriptorPool(Context.GetDevice(), descriptorPool_, nullptr);

	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "cleaned up shader {0}", path_.ToString());
}

const UPath& UShader::Path() const
{
	return path_;
}

VkPipeline UShader::GetGraphicsPipeline() const
{
	return graphicsPipeline_;
}

//VkPipelineLayout UShader::GetPipelineLayout() const
//{
//	return pipelineLayout_;
//}

void UShader::CmdBind(VkCommandBuffer commandBuffer) const
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline_);
}

void UShader::CmdBindDescriptorSets(VkCommandBuffer commandBuffer, const u32 imageIndex) const
{
	std::vector<VkDescriptorSet> descriptorSets{};
	descriptorSets.reserve(descriptorSetLayoutDatas_.size());

	for (const auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		descriptorSets.push_back(descriptorSetLayoutData.descriptorSets[imageIndex]);
	}

	vkCmdBindDescriptorSets(commandBuffer
		, VK_PIPELINE_BIND_POINT_GRAPHICS
		, PipelineResourceManager.GetPipelineLayout()
		, 0
		, static_cast<u32>(descriptorSets.size())
		, descriptorSets.data()
		, 0
		, nullptr
	);
}

void UShader::CreateDescriptorSetLayouts()
{
	descriptorSetLayoutDatas_.reserve(shaderLayout_.descriptorSetLayouts.size());
	for (const auto& [set, setLayout] : shaderLayout_.descriptorSetLayouts)
	{
		const size bindingCount{ setLayout.bindings.size() };

		std::vector<VkDescriptorSetLayoutBinding> setBindings{};
		std::vector<VkDescriptorBindingFlags> setBindingFlags{};
		std::vector<DescriptorSetLayoutBindingData> setBindingDatas{};
		setBindings.reserve(bindingCount);
		setBindingFlags.reserve(bindingCount);
		setBindingDatas.reserve(bindingCount);

		for (const auto& ktBinding : setLayout.bindings)
		{
			u32 descriptorCount;
			if (ktBinding.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
			{
				descriptorCount = MAX_BINDLESS_TEXTURES;
				setBindingFlags.push_back(BINDLESS_TEXTURE_FLAGS);
			}
			else
			{
				descriptorCount = ktBinding.descriptorCount;
				setBindingFlags.push_back(0);
			}

			const VkDescriptorSetLayoutBinding vkBinding{
				.binding = ktBinding.binding,
				.descriptorType = ktBinding.descriptorType,
				.descriptorCount = descriptorCount,
				.stageFlags = ktBinding.shaderStageFlags,
			};
			setBindings.push_back(vkBinding);

			const DescriptorSetLayoutBindingData bindingData{
				.name = ktBinding.name,
				.memberSize = ktBinding.size,
				.descriptorType = ktBinding.descriptorType,
				.binding = ktBinding.binding,
				.descriptorCount = ktBinding.descriptorCount,
				.shaderStageFlags = ktBinding.shaderStageFlags,
				.isBindless = ktBinding.isBindless,
			};
			setBindingDatas.push_back(bindingData);
		}

		VkDescriptorSetLayout newSetLayout;
		CreateDescriptorSetLayout(newSetLayout, setBindings, setBindingFlags);

		const DescriptorSetLayoutData descriptorSetLayoutData{
			.descriptorSetLayout = newSetLayout,
			.descriptorSetLayoutBindingDatas = setBindingDatas,
		};
		descriptorSetLayoutDatas_.push_back(descriptorSetLayoutData);
	}

	for (auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		for (auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.descriptorSetLayoutBindingDatas)
		{
			descriptorSetLayoutBindingDataRegistry_[descriptorSetLayoutBindingData.name] = &descriptorSetLayoutBindingData;
		}
	}
}

void UShader::CreateDescriptorSetLayout(VkDescriptorSetLayout& layout
	, const std::span<VkDescriptorSetLayoutBinding> layoutBindings
	, const std::span<VkDescriptorBindingFlags> bindingFlags)
{
	const VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
		.bindingCount = static_cast<u32>(bindingFlags.size()),
		.pBindingFlags = bindingFlags.data(),
	};

	const VkDescriptorSetLayoutCreateInfo layoutInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext = &bindingFlagsInfo,
		.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT,
		.bindingCount = static_cast<u32>(layoutBindings.size()),
		.pBindings = layoutBindings.data(),
	};

	VK_CHECK_THROW(
		vkCreateDescriptorSetLayout(Context.GetDevice(), &layoutInfo, nullptr, &layout),
		"failed to create descriptor set layout!"
	);
}

void UShader::CreateDescriptorSets()
{
	for (auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		const auto variableDescriptorCounts{ make_frames_in_flight_array(MAX_BINDLESS_TEXTURES) };

		const VkDescriptorSetVariableDescriptorCountAllocateInfo countInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
			.descriptorSetCount = static_cast<u32>(variableDescriptorCounts.size()),
			.pDescriptorCounts = variableDescriptorCounts.data(),
		};

		const auto layouts{ make_frames_in_flight_array(descriptorSetLayoutData.descriptorSetLayout) };

		const VkDescriptorSetAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext = &countInfo,
			.descriptorPool = descriptorPool_,
			.descriptorSetCount = static_cast<u32>(layouts.size()),
			.pSetLayouts = layouts.data(),
		};

		VK_CHECK_THROW(
			vkAllocateDescriptorSets(Context.GetDevice(), &allocInfo, descriptorSetLayoutData.descriptorSets.data()),
			"failed to allocate descriptor sets!"
		);

		for (auto& bindingData : descriptorSetLayoutData.descriptorSetLayoutBindingDatas)
		{
			bindingData.descriptorSetLayout = descriptorSetLayoutData.descriptorSetLayout;
			bindingData.descriptorSets = descriptorSetLayoutData.descriptorSets;
		}
	}
}

void UShader::CreateDescriptorSetLayoutBindings()
{
	for (auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		for (auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.descriptorSetLayoutBindingDatas)
		{
			for (size i{ 0 }; i < KT_FRAMES_IN_FLIGHT; i++)
			{
				UpdateDescriptorSetLayoutBindingBufferMemberCount(descriptorSetLayoutBindingData, 1, static_cast<u32>(i));
				CreateDescriptorSetLayoutBindingImageSampler(descriptorSetLayoutBindingData, static_cast<u32>(i));
			}
		}
	}
}

void UShader::CreateDescriptorPool(const std::span<VkDescriptorPoolSize> poolSizes
	, const u32 setCount)
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

void UShader::CreateShaderModule(VkShaderModule& shaderModule, const std::span<u8> code)
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

void UShader::CreateGraphicsPipeline()
{
	//std::vector<VkDescriptorSetLayout> setLayouts{};
	//for (const auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	//{
	//	setLayouts.push_back(descriptorSetLayoutData.descriptorSetLayout);
	//}
	//
	//const VkPipelineLayoutCreateInfo pipelineLayoutInfo{
	//	.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
	//	.setLayoutCount = static_cast<u32>(setLayouts.size()),
	//	.pSetLayouts = setLayouts.data(),
	//	.pushConstantRangeCount = 0, // Optional
	//	.pPushConstantRanges = nullptr, // Optional
	//};
	//VK_CHECK_THROW(
	//	vkCreatePipelineLayout(Context.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout_),
	//	"failed to create pipeline layout!"
	//);


	std::vector<VkShaderModule> shaderModules;
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	nlohmann::json json{};
	USerializer::Deserialize(json, path_);

	for (const auto& shader : json["shaders"])
	{
		const auto path{ UPath{ "${ENGINE_DIRECTORY}/Graphics/shaders" }.ToPath() / shader["path"] };
		std::vector shaderCode{ UFile{ path }.ReadBinary() };

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
		
		.vertexBindingDescriptionCount = 0,
		//.vertexBindingDescriptionCount = static_cast<u32>(shaderLayout_.vertexInputBindingDescriptions.size()),
		//.pVertexBindingDescriptions = shaderLayout_.vertexInputBindingDescriptions.data(),
		
		.vertexAttributeDescriptionCount = 0,
		//.vertexAttributeDescriptionCount = static_cast<u32>(shaderLayout_.vertexInputAttributeDescriptions.size()),
		//.pVertexAttributeDescriptions = shaderLayout_.vertexInputAttributeDescriptions.data(),
	};

	const VkPipelineInputAssemblyStateCreateInfo inputAssembly{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE,
	};

	const VkViewport viewport{
		.x = 0.0f,
		.y = 0.0f,
		.width = (float)Renderer.GetSwapChainExtent().width,
		.height = (float)Renderer.GetSwapChainExtent().height,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};

	const VkRect2D scissor{
		.offset = { 0, 0 },
		.extent = Renderer.GetSwapChainExtent(),
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

	const std::vector<VkDynamicState> dynamicStates
	{ 
		VK_DYNAMIC_STATE_VIEWPORT, 
		VK_DYNAMIC_STATE_SCISSOR 
	};
	const VkPipelineDynamicStateCreateInfo dynamicState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = static_cast<u32>(dynamicStates.size()),
		.pDynamicStates = dynamicStates.data(),
	};

	const std::array colorAttachmentFormats{ Renderer.GetSwapChainFormat() };
	const VkPipelineRenderingCreateInfo pipelineRenderingInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.pNext = VK_NULL_HANDLE,
		.colorAttachmentCount = static_cast<u32>(colorAttachmentFormats.size()),
		.pColorAttachmentFormats = colorAttachmentFormats.data(),
		.depthAttachmentFormat = Renderer.GetDepthFormat(),
	};

	const VkGraphicsPipelineCreateInfo pipelineInfo{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.pNext = &pipelineRenderingInfo,
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

		.layout = PipelineResourceManager.GetPipelineLayout(),
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

void UShader::CreateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex)
{
	if (const auto bufferUsageFlagBits{ GetBufferUsageFlagBits(descriptorSetLayoutBindingData.descriptorType) })
	{
		Context.CreateBuffer(
			descriptorSetLayoutBindingData.memberSize * descriptorSetLayoutBindingData.memberCounts[imageIndex],
			bufferUsageFlagBits | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			0,
			descriptorSetLayoutBindingData.buffers[imageIndex],
			VMA_MEMORY_USAGE_GPU_ONLY
		);

		Context.CreateBuffer(
			descriptorSetLayoutBindingData.memberSize * descriptorSetLayoutBindingData.memberCounts[imageIndex],
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VMA_ALLOCATION_CREATE_MAPPED_BIT,
			descriptorSetLayoutBindingData.stagingBuffers[imageIndex],
			VMA_MEMORY_USAGE_CPU_TO_GPU
		);
	}
}

void UShader::CreateDescriptorSetLayoutBindingImageSampler(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex)
{
	if (!GetIsImageSamplerDescriptorType(descriptorSetLayoutBindingData.descriptorType))
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "descriptor type {0} isn't an image sampler type", static_cast<u32>(descriptorSetLayoutBindingData.descriptorType));
		return;
	}

	static const UAsset texture{ UAssetManager<UTexture>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/viking_room.png") };

	UpdateDescriptorSetLayoutBindingImageSampler(descriptorSetLayoutBindingData, { texture->GetDescriptorImageInfo() }, imageIndex);
}

void UShader::DebugLogDescriptorSetLayoutData() const
{
	for (const auto& descriptorSetLayoutData : descriptorSetLayoutDatas_)
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "DescriptorSetLayout: {0}", static_cast<void*>(descriptorSetLayoutData.descriptorSetLayout));
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| DescriptorSets");
		for (const auto& descriptorSet : descriptorSetLayoutData.descriptorSets)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | {0}", static_cast<void*>(descriptorSet));
		}
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| DescriptorSetLayoutBindingDatas");
		for (const auto& descriptorSetLayoutBindingData : descriptorSetLayoutData.descriptorSetLayoutBindingDatas)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | Name: {0}", descriptorSetLayoutBindingData.name);
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | Binding: {0}", descriptorSetLayoutBindingData.binding);
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | DescriptorCount: {0}", descriptorSetLayoutBindingData.descriptorCount);
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | DescriptorType: {0}", static_cast<u32>(descriptorSetLayoutBindingData.descriptorType));
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | MemberSize: {0}", descriptorSetLayoutBindingData.memberSize);
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | StageFlags: {0}", descriptorSetLayoutBindingData.shaderStageFlags);
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "| | Buffers");
			for (const auto& buffer : descriptorSetLayoutBindingData.buffers)
			{
				KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics.DebugLogDescriptorSetLayoutData()", "| | | {0}", (void*)(&buffer));
			}
			for (const auto& stagingBuffer : descriptorSetLayoutBindingData.stagingBuffers)
			{
				KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics.DebugLogDescriptorSetLayoutData()", "| | | {0}", (void*)(&stagingBuffer));
			}
		}
	}
}

bool UShader::GetIsBufferDescriptorType(const VkDescriptorType descriptorType) const
{
	return descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER 
		|| descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER 
		|| descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC 
		|| descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER 
		|| descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC 
		|| descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
}

bool UShader::GetIsImageSamplerDescriptorType(const VkDescriptorType descriptorType) const
{
	return descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
}

VkBufferUsageFlagBits UShader::GetBufferUsageFlagBits(const VkDescriptorType descriptorType) const
{
	switch (descriptorType)
	{
	case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:	return VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
	case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:	return VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
	case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:	
	case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:			return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:	
	case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:			return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	default:										return {};
	}
}

void UShader::UpdateDescriptorSetLayoutBindingBuffer(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const void* data, const u32 imageIndex)
{
	const size dataSize{ descriptorSetLayoutBindingData.memberSize * descriptorSetLayoutBindingData.memberCounts[imageIndex] };

	memcpy(descriptorSetLayoutBindingData.stagingBuffers[imageIndex].AllocationInfo.pMappedData, data, dataSize);

	Context.CopyBuffer(
		descriptorSetLayoutBindingData.stagingBuffers[imageIndex].Buffer,
		descriptorSetLayoutBindingData.buffers[imageIndex].Buffer,
		dataSize
	);
}

void UShader::UpdateDescriptorSetLayoutBindingBufferMemberCount(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const size memberCount, const u32 imageIndex)
{
	if (!GetIsBufferDescriptorType(descriptorSetLayoutBindingData.descriptorType))
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "descriptor type {0} isn't a buffer type", static_cast<u32>(descriptorSetLayoutBindingData.descriptorType));
		return;
	}

	if (descriptorSetLayoutBindingData.memberCounts[imageIndex] == memberCount)
	{
		return;
	}

	KT_LOG(ELogImportanceLevel::Low, "Graphics", "descriptorSetLayoutBindingData member count at frame {}: {}", imageIndex, memberCount);
	descriptorSetLayoutBindingData.memberCounts[imageIndex] = std::max(1llu, memberCount);

	vmaDestroyBuffer(Context.GetAllocator(), descriptorSetLayoutBindingData.buffers[imageIndex].Buffer, descriptorSetLayoutBindingData.buffers[imageIndex].Allocation);
	vmaDestroyBuffer(Context.GetAllocator(), descriptorSetLayoutBindingData.stagingBuffers[imageIndex].Buffer, descriptorSetLayoutBindingData.stagingBuffers[imageIndex].Allocation);

	CreateDescriptorSetLayoutBindingBuffer(descriptorSetLayoutBindingData, imageIndex);

	UpdateDescriptorSetLayoutBindingBufferDescriptorSet(descriptorSetLayoutBindingData, imageIndex);
}

void UShader::UpdateDescriptorSetLayoutBindingImageSampler(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData
	, const std::vector<VkDescriptorImageInfo>& imageInfos
	, const u32 imageIndex)
{
	if (!GetIsImageSamplerDescriptorType(descriptorSetLayoutBindingData.descriptorType))
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "descriptor type {0} isn't an image sampler type", static_cast<u32>(descriptorSetLayoutBindingData.descriptorType));
		return;
	}

	const auto& currentImageInfos{ descriptorSetLayoutBindingData.imageInfos[imageIndex] };

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
		descriptorSetLayoutBindingData.descriptorCount = static_cast<u32>(imageInfos.size());
		descriptorSetLayoutBindingData.imageInfos[imageIndex] = imageInfos;
		UpdateDescriptorSetLayoutBindingImageSamplerDescriptorSet(descriptorSetLayoutBindingData, imageIndex);
	}
}

void UShader::UpdateDescriptorSetLayoutBindingBufferDescriptorSet(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex)
{
	const VkDescriptorBufferInfo bufferInfo{
		.buffer = descriptorSetLayoutBindingData.buffers[imageIndex].Buffer,
		.offset = 0,
		.range = descriptorSetLayoutBindingData.memberSize * descriptorSetLayoutBindingData.memberCounts[imageIndex],
	};

	const VkWriteDescriptorSet writeDescriptorSet{
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = descriptorSetLayoutBindingData.descriptorSets[imageIndex],
		.dstBinding = descriptorSetLayoutBindingData.binding,
		.dstArrayElement = 0,
		.descriptorCount = descriptorSetLayoutBindingData.descriptorCount,
		.descriptorType = descriptorSetLayoutBindingData.descriptorType,
		.pBufferInfo = &bufferInfo,
	};

	vkUpdateDescriptorSets(Context.GetDevice(), 1, &writeDescriptorSet, 0, nullptr);
}

void UShader::UpdateDescriptorSetLayoutBindingImageSamplerDescriptorSet(DescriptorSetLayoutBindingData& descriptorSetLayoutBindingData, const u32 imageIndex)
{
	VkWriteDescriptorSet writeDescriptorSet{
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = descriptorSetLayoutBindingData.descriptorSets[imageIndex],
		.dstBinding = descriptorSetLayoutBindingData.binding,
		.dstArrayElement = 0,
		.descriptorCount = descriptorSetLayoutBindingData.descriptorCount,
		.descriptorType = descriptorSetLayoutBindingData.descriptorType,
		.pImageInfo = descriptorSetLayoutBindingData.imageInfos[imageIndex].data(),
	};
	vkUpdateDescriptorSets(Context.GetDevice(), 1, &writeDescriptorSet, 0, nullptr);
}

UShader::DescriptorSetLayoutBindingData* UShader::GetDescriptorSetLayoutBinding(const std::string& name)
{
	const auto it{ descriptorSetLayoutBindingDataRegistry_.find(name) };
	if (it != descriptorSetLayoutBindingDataRegistry_.end())
	{
		return it->second;
	}
	return nullptr;
}

void UShader::CreateDescriptorPools()
{
	std::vector<VkDescriptorPoolSize> poolSizes{};
	for (const auto& [index, setLayout] : shaderLayout_.descriptorSetLayouts)
	{
		for (const auto& binding : setLayout.bindings)
		{
			const u32 descriptorCount{ binding.descriptorCount > 1
				? static_cast<u32>(KT_FRAMES_IN_FLIGHT) * binding.descriptorCount
				: static_cast<u32>(KT_FRAMES_IN_FLIGHT) 
			};

			const VkDescriptorPoolSize poolSize{
				.type = binding.descriptorType,
				.descriptorCount = descriptorCount,
			};
			poolSizes.push_back(poolSize);
		}
	}
	CreateDescriptorPool(poolSizes, static_cast<u32>(shaderLayout_.descriptorSetLayouts.size()));
}

void UShader::CreateShaderLayout()
{
	nlohmann::json json{};
	USerializer::Deserialize(json, path_);
	for (const auto& shader : json["shaders"])
	{
		const auto path{ UPath{ "${ENGINE_DIRECTORY}/Graphics/shaders" }.ToPath() / shader["path"] };
		std::vector shaderCode{ UFile{ path }.ReadBinary() };
		shaderLayout_.Populate(shaderCode, shader["shaderStage"]);
	}
}
