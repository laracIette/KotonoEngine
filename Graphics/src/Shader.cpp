#include "Shader.h"
#include "PipelineResourceManager.h"
#include "Renderer.h"
#include "SwapChain.h"
#include <kotono_common/log.h>
#include <kotono_io/File.h>
#include <kotono_io/Serializer.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>
#include <nlohmann/json.hpp>

#define KT_LOG_IMPORTANCE_LEVEL_SHADER ELogImportanceLevel::High

UShader::UShader(const UPath& path) 
	: path_{ path }
	, pipeline_{ VK_NULL_HANDLE }
{
	nlohmann::json json{};
	SSerializer::Deserialize(json, path_);

	if (json["pipelinePass"] == EPipelinePass::Compute)
	{
		CreateComputePipeline();
	}
	else
	{
		CreateGraphicsPipeline();
	}
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "initialized shader {0}", path_.ToString());
}

UShader::~UShader()
{
	vkDestroyPipeline(Context.GetDevice(), pipeline_, nullptr);
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SHADER, "Graphics", "cleaned up shader {0}", path_.ToString());
}

const UPath& UShader::Path() const
{
	return path_;
}

VkPipeline UShader::GetPipeline() const
{
	return pipeline_;
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
	std::vector<VkShaderModule> shaderModules;
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	nlohmann::json json{};
	SSerializer::Deserialize(json, path_);

	for (const auto& shader : json["shaders"])
	{
		const UPath path{ shader["path"] };
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

	const VkViewport viewport{
		.x = 0.0f,
		.y = 0.0f,
		.width = static_cast<float>(SwapChain.GetExtent().width),
		.height = static_cast<float>(SwapChain.GetExtent().height),
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};

	const VkRect2D scissor{
		.offset = { 0, 0 },
		.extent = SwapChain.GetExtent(),
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

	const std::vector colorAttachmentFormats{ GetOutputColorAttachmentFormats(json["pipelinePass"]) };
	const VkPipelineRenderingCreateInfo pipelineRenderingInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.pNext = VK_NULL_HANDLE,
		.viewMask = json["viewMask"],
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
		vkCreateGraphicsPipelines(Context.GetDevice(), nullptr, 1, &pipelineInfo, nullptr, &pipeline_),
		"failed to create graphics pipeline!"
	);

	for (auto shaderModule : shaderModules)
	{
		vkDestroyShaderModule(Context.GetDevice(), shaderModule, nullptr);
	}
}

void UShader::CreateComputePipeline()
{
	nlohmann::json json{};
	SSerializer::Deserialize(json, path_);

	const UPath path{ json["path"] };
	std::vector shaderCode{ UFile{ path }.ReadBinary() };

	VkShaderModule shaderModule;
	CreateShaderModule(shaderModule, shaderCode);

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
		.layout = PipelineResourceManager.GetPipelineLayout(),
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1,
	};

	VK_CHECK_THROW(
		vkCreateComputePipelines(Context.GetDevice(), nullptr, 1, &pipelineInfo, nullptr, &pipeline_),
		"failed to create compute pipeline!"
	);

	vkDestroyShaderModule(Context.GetDevice(), shaderModule, nullptr);
}

std::vector<VkFormat> UShader::GetOutputColorAttachmentFormats(const EPipelinePass pipelinePass) const
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
	case EPipelinePass::Interface:			return { SwapChain.GetFormat() };
	default:								return {};
	}
}
