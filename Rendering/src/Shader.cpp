#include "Shader.h"

#include <kotono_common/log.h>
#include <kotono_io/File.h>
#include <kotono_io/Serializer.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>
#include <nlohmann/json.hpp>

#define KT_LOG_IMPORTANCE_LEVEL_SHADER ELogImportanceLevel::High

AShader::AShader(UPath const& path)
	: AAsset(path)
	, pipeline_{ VK_NULL_HANDLE }
{
}

AShader::~AShader()
{
	vkDestroyPipeline(Context.GetDevice(), pipeline_, nullptr);
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "cleaned up shader {0}", GetPath().ToString());
}

void AShader::Init(VkFormat swapChainFormat, VkPipelineLayout pipelineLayout)
{
	nlohmann::json json{};
	SSerializer::Deserialize(json, GetPath());

	if (json["pipelinePass"] == EPipelinePass::Compute)
	{
		CreateComputePipeline(pipelineLayout);
	}
	else
	{
		CreateGraphicsPipeline(swapChainFormat, pipelineLayout);
	}
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "initialized shader {0}", GetPath().ToString());
}

VkPipeline AShader::GetPipeline() const
{
	return pipeline_;
}

VkShaderModule AShader::CreateShaderModule(std::span<u8 const> code)
{
	const VkShaderModuleCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size(),
		.pCode = reinterpret_cast<const u32*>(code.data()),
	};

	VkShaderModule shaderModule;

	VK_CHECK_THROW(
		vkCreateShaderModule(Context.GetDevice(), &createInfo, nullptr, &shaderModule),
		"failed to create shader module!"
	);

	return shaderModule;
}

void AShader::CreateGraphicsPipeline(VkFormat swapChainFormat, VkPipelineLayout pipelineLayout)
{
	std::vector<VkShaderModule> shaderModules;
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	nlohmann::json json{};
	SSerializer::Deserialize(json, GetPath());

	for (auto const& shader : json["shaders"])
	{
		UPath const path{ shader["path"] };
		std::vector const shaderCode{ UFile{ path }.ReadBinary() };

		VkShaderModule const shaderModule{ CreateShaderModule(shaderCode) };
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
	const bool useMSAASamples{ dataMultisampling["useMSAASamples"] };
	const VkSampleCountFlagBits rasterizationSamples{ dataMultisampling["rasterizationSamples"] };
	const VkPipelineMultisampleStateCreateInfo multisampling{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = useMSAASamples ? Context.GetMSAASamples() : rasterizationSamples,
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

	std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments{};
	for (const auto& dataColorBlendAttachment : json["colorBlendAttachments"])
	{
		const VkPipelineColorBlendAttachmentState colorBlendAttachment{
		   .blendEnable = dataColorBlendAttachment["blendEnable"],
		   .srcColorBlendFactor = dataColorBlendAttachment["srcColorBlendFactor"],
		   .dstColorBlendFactor = dataColorBlendAttachment["dstColorBlendFactor"],
		   .colorBlendOp = dataColorBlendAttachment["colorBlendOp"],
		   .srcAlphaBlendFactor = dataColorBlendAttachment["srcAlphaBlendFactor"],
		   .dstAlphaBlendFactor = dataColorBlendAttachment["dstAlphaBlendFactor"],
		   .alphaBlendOp = dataColorBlendAttachment["alphaBlendOp"],
		   .colorWriteMask = dataColorBlendAttachment["colorWriteMask"],
		};
		colorBlendAttachments.push_back(colorBlendAttachment);
	}

	const VkPipelineVertexInputStateCreateInfo vertexInputInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 0,
		.vertexAttributeDescriptionCount = 0,
	};

	const VkPipelineInputAssemblyStateCreateInfo inputAssembly{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE,
	};

	const VkPipelineViewportStateCreateInfo viewportState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = nullptr, // Dynamic state viewport
		.scissorCount = 1,
		.pScissors = nullptr, // Dynamic state scissor
	};

	const VkPipelineColorBlendStateCreateInfo colorBlending{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY, // Optional
		.attachmentCount = static_cast<u32>(colorBlendAttachments.size()),
		.pAttachments = colorBlendAttachments.data(),
		.blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f }, // Optional
	};

	constexpr std::array dynamicStates
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	const VkPipelineDynamicStateCreateInfo dynamicState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = static_cast<u32>(dynamicStates.size()),
		.pDynamicStates = dynamicStates.data(),
	};

	const std::vector colorAttachmentFormats{ GetOutputColorAttachmentFormats(json["pipelinePass"], swapChainFormat) };
	const VkPipelineRenderingCreateInfo pipelineRenderingInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.pNext = VK_NULL_HANDLE,
		.viewMask = json["viewMask"],
		.colorAttachmentCount = static_cast<u32>(colorAttachmentFormats.size()),
		.pColorAttachmentFormats = colorAttachmentFormats.data(),
		.depthAttachmentFormat = Context.GetDepthFormat(),
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

		.layout = pipelineLayout,
	};
	VK_CHECK_THROW(
		vkCreateGraphicsPipelines(Context.GetDevice(), nullptr, 1, &pipelineInfo, nullptr, &pipeline_),
		"failed to create graphics pipeline!"
	);

	for (auto shaderModule : shaderModules)
	{
		vkDestroyShaderModule(Context.GetDevice(), shaderModule, nullptr);
	}
}

void AShader::CreateComputePipeline(VkPipelineLayout pipelineLayout)
{
	nlohmann::json json{};
	SSerializer::Deserialize(json, GetPath());

	UPath const path{ json["path"] };
	std::vector const shaderCode{ UFile{ path }.ReadBinary() };

	VkShaderModule const shaderModule{ CreateShaderModule(shaderCode) };

	const VkPipelineShaderStageCreateInfo shaderStageInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.pNext = VK_NULL_HANDLE,
		.flags = 0,
		.stage = VK_SHADER_STAGE_COMPUTE_BIT,
		.module = shaderModule,
		.pName = "main",
		.pSpecializationInfo = VK_NULL_HANDLE,
	};

	const VkComputePipelineCreateInfo pipelineInfo{
		.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
		.pNext = VK_NULL_HANDLE,
		.flags = 0,
		.stage = shaderStageInfo,
		.layout = pipelineLayout,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1,
	};

	VK_CHECK_THROW(
		vkCreateComputePipelines(Context.GetDevice(), nullptr, 1, &pipelineInfo, nullptr, &pipeline_),
		"failed to create compute pipeline!"
	);

	vkDestroyShaderModule(Context.GetDevice(), shaderModule, nullptr);
}

std::vector<VkFormat> AShader::GetOutputColorAttachmentFormats(EPipelinePass pipelinePass, VkFormat swapChainFormat) const
{
	switch (pipelinePass)
	{
	case EPipelinePass::Compute:
	case EPipelinePass::ShadowPrePass:
	case EPipelinePass::DepthPrePass:		return {};
	case EPipelinePass::GBuffer:			return {
		VK_FORMAT_R8G8B8A8_SRGB,		// Albedo
		VK_FORMAT_R16G16B16A16_SFLOAT,	// Normal
		VK_FORMAT_R8G8B8A8_UNORM		// ORM
	};
	case EPipelinePass::DeferredLighting:	return { VK_FORMAT_R16G16B16A16_SFLOAT };
	case EPipelinePass::PostProcess:
	case EPipelinePass::Interface:			return { swapChainFormat };
	default:								return {};
	}
}
