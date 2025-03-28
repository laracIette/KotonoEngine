#include "Shader2D.h"
#include <vulkan/vulkan.h>
#include "Vertex2D.h"
#include "File.h"
#include "Framework.h"
#include "log.h"
#include "vk_utils.h"

void KtShader2D::Init()
{
	SetName("2D Shader");
	SetVertPath(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2DVert.spv)");
	SetFragPath(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2DFrag.spv)");
	KtShader::Init();
}

void KtShader2D::CreateGraphicsPipelines()
{
	std::array<VkVertexInputBindingDescription, 1> bindingDescriptions =
	{ 
		VkVertexInputBindingDescription { 0, sizeof(KtVertex2D), VK_VERTEX_INPUT_RATE_VERTEX }
	};
	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions =
	{
		VkVertexInputAttributeDescription { 0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(KtVertex2D, Position) },
		VkVertexInputAttributeDescription { 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(KtVertex2D, Color) },
		VkVertexInputAttributeDescription { 2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(KtVertex2D, TexCoord) }
	};

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f; // For rasterizer.polygonMode = VK_POLYGON_MODE_LINE; 
	// enable wideLines GPU feature for thicker
	rasterizer.cullMode = VK_CULL_MODE_NONE; // diff from 3D
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_TRUE; // enable sample shading in the pipeline
	multisampling.rasterizationSamples = Framework.GetContext().GetMSAASamples();
	multisampling.minSampleShading = 0.2f; // min fraction for sample shading; closer to one is smoother
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_FALSE; // diff from 3D
	depthStencil.depthWriteEnable = VK_FALSE; // diff from 3D
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE; // diff from 3D
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA; // diff from 3D
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; // diff from 3D
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // diff from 3D
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // diff from 3D
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // diff from 3D
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // diff from 3D
	
	std::vector<VkDescriptorSetLayout> setLayouts;
	for (const auto& descriptorSetLayoutData : _descriptorSetLayoutDatas)
	{
		setLayouts.push_back(descriptorSetLayoutData.DescriptorSetLayout);
	}

	CreateGraphicsPipeline(bindingDescriptions, attributeDescriptions, rasterizer, multisampling, depthStencil, colorBlendAttachment, setLayouts);
}