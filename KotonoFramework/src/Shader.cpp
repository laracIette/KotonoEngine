#include "Shader.h"
#include "vk_utils.h"
#include "Framework.h"
#include "File.h"
#include <spirv-reflect/spirv_reflect.h>

void KtShader::Init()
{
	CreateDescriptorSetLayouts();
	CreateDescriptorPools();
	CreateUniformBuffers();
	CreateObjectBuffers();
	CreateDescriptorSets();
	CreateGraphicsPipelines();
}

void KtShader::Cleanup()
{
	KT_DEBUG_LOG("cleaning up shader");

	vkDestroyPipeline(Framework.GetContext().GetDevice(), _graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(Framework.GetContext().GetDevice(), _pipelineLayout, nullptr);

	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		vmaDestroyBuffer(Framework.GetContext().GetAllocator(), _uniformBuffers[i].Buffer, _uniformBuffers[i].Allocation);
		vmaDestroyBuffer(Framework.GetContext().GetAllocator(), _stagingUniformBuffers[i].Buffer, _stagingUniformBuffers[i].Allocation);
		vmaDestroyBuffer(Framework.GetContext().GetAllocator(), _objectBuffers[i].Buffer, _objectBuffers[i].Allocation);
		vmaDestroyBuffer(Framework.GetContext().GetAllocator(), _stagingObjectBuffers[i].Buffer, _stagingObjectBuffers[i].Allocation);
	}

	vkDestroyDescriptorSetLayout(Framework.GetContext().GetDevice(), _uniformDescriptorSetLayout, nullptr);
	vkDestroyDescriptorSetLayout(Framework.GetContext().GetDevice(), _objectDescriptorSetLayout, nullptr);

	vkDestroyDescriptorPool(Framework.GetContext().GetDevice(), _descriptorPool, nullptr);

	KT_DEBUG_LOG("cleaned up shader");
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
	vkCmdBindDescriptorSets(
		commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout,
		0, 1, &_uniformDescriptorSets[imageIndex], 0, nullptr
	);

	vkCmdBindDescriptorSets(
		commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout,
		1, 1, &_objectDescriptorSets[imageIndex], 0, nullptr
	);
}

void KtShader::SetVertPath(const std::filesystem::path& path)
{
	_vertPath = path;
}

void KtShader::SetFragPath(const std::filesystem::path& path)
{
	_fragPath = path;
}

void KtShader::SetUniformDataSize(const VkDeviceSize size)
{
	_uniformDataSize = size;
}

void KtShader::SetObjectDataSize(const VkDeviceSize size)
{
	_objectDataSize = size;
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
	std::array<VkDescriptorSetLayout, KT_FRAMES_IN_FLIGHT> uniformLayouts{};
	uniformLayouts.fill(_uniformDescriptorSetLayout);

	std::array<VkDescriptorSetLayout, KT_FRAMES_IN_FLIGHT> objectLayouts{};
	objectLayouts.fill(_objectDescriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(uniformLayouts.size());
	allocInfo.pSetLayouts = uniformLayouts.data();

	VK_CHECK_THROW(
		vkAllocateDescriptorSets(Framework.GetContext().GetDevice(), &allocInfo, _uniformDescriptorSets.data()),
		"failed to allocate uniform descriptor sets!"
	);

	allocInfo.descriptorSetCount = static_cast<uint32_t>(objectLayouts.size());
	allocInfo.pSetLayouts = objectLayouts.data();
	VK_CHECK_THROW(
		vkAllocateDescriptorSets(Framework.GetContext().GetDevice(), &allocInfo, _objectDescriptorSets.data()),
		"failed to allocate object descriptor sets!"
	);

	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		UpdateDescriptorSet(static_cast<uint32_t>(i));
	}
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

	ReadSPV(vertShaderCode);
	ReadSPV(fragShaderCode);

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

	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
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

	vkDestroyShaderModule(Framework.GetContext().GetDevice(), fragShaderModule, nullptr);
	vkDestroyShaderModule(Framework.GetContext().GetDevice(), vertShaderModule, nullptr);
}

void KtShader::CreateUniformBuffers()
{
	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		CreateUniformBuffer(static_cast<uint32_t>(i));
	}
}

void KtShader::CreateObjectBuffers()
{
	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		CreateObjectBuffer(static_cast<uint32_t>(i));
	}
}

void KtShader::CreateUniformBuffer(const uint32_t imageIndex)
{
	Framework.GetContext().CreateBuffer(
		_uniformDataSize,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0,
		_uniformBuffers[imageIndex],
		VMA_MEMORY_USAGE_GPU_ONLY
	);

	Framework.GetContext().CreateBuffer(
		_uniformDataSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_MAPPED_BIT,
		_stagingUniformBuffers[imageIndex],
		VMA_MEMORY_USAGE_CPU_TO_GPU
	);
}

void KtShader::CreateObjectBuffer(const uint32_t imageIndex)
{
	KT_DEBUG_LOG("Object buffer size at frame %u: %llu", imageIndex, GetObjectBufferCount(imageIndex) * _objectDataSize);
	Framework.GetContext().CreateBuffer(
		GetObjectBufferCount(imageIndex) * _objectDataSize,
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, // Can be used as SSBO & can receive data from staging
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, // Optimized for GPU access
		0, // No need for MAPPED_BIT since we won’t access this from CPU
		_objectBuffers[imageIndex],
		VMA_MEMORY_USAGE_GPU_ONLY // Best for performance
	);

	Framework.GetContext().CreateBuffer(
		GetObjectBufferCount(imageIndex) * _objectDataSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, // Usage flags
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // Memory properties
		VMA_ALLOCATION_CREATE_MAPPED_BIT, // Allocation flags
		_stagingObjectBuffers[imageIndex],
		VMA_MEMORY_USAGE_CPU_TO_GPU
	);
}

void KtShader::SetObjectCount(const VkDeviceSize objectCount, const uint32_t imageIndex)
{
	if (_objectCounts[imageIndex] != objectCount)
	{
		_objectCounts[imageIndex] = objectCount;
		KT_DEBUG_LOG("Shader '%s' object count at frame %u: %llu", _name.c_str(), imageIndex, objectCount);

		vmaDestroyBuffer(Framework.GetContext().GetAllocator(), _objectBuffers[imageIndex].Buffer, _objectBuffers[imageIndex].Allocation);
		vmaDestroyBuffer(Framework.GetContext().GetAllocator(), _stagingObjectBuffers[imageIndex].Buffer, _stagingObjectBuffers[imageIndex].Allocation);

		CreateObjectBuffer(imageIndex);
		UpdateDescriptorSet(imageIndex);
	}
}

const VkDeviceSize KtShader::GetObjectBufferCount(const uint32_t imageIndex) const
{
	return _objectCounts[imageIndex] == 0 ? 1 : _objectCounts[imageIndex];
}

void KtShader::ReadSPV(const std::span<char> data) const
{
	SpvReflectShaderModule module;
	SpvReflectResult result = spvReflectCreateShaderModule(data.size() * sizeof(char), data.data(), &module, 0);

	uint32_t count = 0;
	spvReflectEnumerateDescriptorBindings(&module, &count, nullptr);
	std::vector<SpvReflectDescriptorBinding*> bindings(count);
	spvReflectEnumerateDescriptorBindings(&module, &count, bindings.data());

	for (auto* binding : bindings)
	{
		printf("Set: %d, Binding: %d, Type: %d\n",
			binding->set, binding->binding, binding->descriptor_type);
	}
	spvReflectDestroyShaderModule(&module);
}