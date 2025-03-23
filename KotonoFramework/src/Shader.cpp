#include "Shader.h"
#include "vk_utils.h"
#include "Framework.h"
#include <File.h>

void KtShader::Cleanup()
{
	vkDestroyPipeline(Framework.GetContext().GetDevice(), _graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(Framework.GetContext().GetDevice(), _pipelineLayout, nullptr);
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

void KtShader::SetVertPath(const std::filesystem::path& path)
{
	_vertPath = path;
}

void KtShader::SetFragPath(const std::filesystem::path& path)
{
	_fragPath = path;
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

void KtShader::CreateDescriptorPool(VkDescriptorPool& pool, const std::span<VkDescriptorPoolSize> poolSizes, const uint32_t setCount)
{
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT) * setCount;

	VK_CHECK_THROW(
		vkCreateDescriptorPool(Framework.GetContext().GetDevice(), &poolInfo, nullptr, &pool),
		"failed to create descriptor pool!"
	);
}

void KtShader::CreateShaderModule(VkShaderModule& shaderModule, const std::span<char> code)
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

void KtShader::CreateGraphicsPipeline(
	const std::span<VkVertexInputBindingDescription> bindingDescriptions,
	const std::span<VkVertexInputAttributeDescription> attributeDescriptions,
	const VkPipelineRasterizationStateCreateInfo& rasterizer,
	const VkPipelineMultisampleStateCreateInfo& multisampling,
	const VkPipelineDepthStencilStateCreateInfo& depthStencil,
	const VkPipelineColorBlendAttachmentState& colorBlendAttachment,
	const std::span<VkDescriptorSetLayout> setLayouts
)
{
	std::vector<char> vertShaderCode = KtFile(_vertPath).GetBinaryContent();
	std::vector<char> fragShaderCode = KtFile(_fragPath).GetBinaryContent();

	VkShaderModule vertShaderModule;
	CreateShaderModule(vertShaderModule, vertShaderCode);
	VkShaderModule fragShaderModule;
	CreateShaderModule(fragShaderModule, fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages =
	{ 
		vertShaderStageInfo, 
		fragShaderStageInfo 
	};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	vertexInputInfo.vertexBindingDescriptionCount = bindingDescriptions.size();
	vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
	vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

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
	pipelineInfo.stageCount = shaderStages.size();
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

	vkDestroyShaderModule(Framework.GetContext().GetDevice(), fragShaderModule, nullptr);
	vkDestroyShaderModule(Framework.GetContext().GetDevice(), vertShaderModule, nullptr);
}
