#include "Shader.h"
#include <vulkan/vulkan.h>
#include "Vertex.h"
#include "File.h"
#include "Framework.h"
#include "log.h"
#include "vk_utils.h"

KtShader::KtShader(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath) :
	_vertPath(vertPath),
	_fragPath(fragPath)
{
	CreateDescriptorSetLayout();
	CreateDescriptorPool();
	CreateImageTexture();
	CreateUniformBuffers();
	CreateObjectBuffers();
	CreateDescriptorSets();
	CreateGraphicsPipeline();
}

KtShader::~KtShader()
{
	KT_DEBUG_LOG("cleaning up shader");
	delete _imageTexture;

	vkDestroyPipeline(Framework.GetWindow().GetContext().GetDevice(), _graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(Framework.GetWindow().GetContext().GetDevice(), _pipelineLayout, nullptr);
	
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), _uniformBuffers[i].Buffer, _uniformBuffers[i].Allocation);
		vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), _stagingUniformBuffers[i].Buffer, _stagingUniformBuffers[i].Allocation);
		vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), _objectBuffers[i].Buffer, _objectBuffers[i].Allocation);
		vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), _stagingObjectBuffers[i].Buffer, _stagingObjectBuffers[i].Allocation);
	}

	vkDestroyDescriptorSetLayout(Framework.GetWindow().GetContext().GetDevice(), _uniformDescriptorSetLayout, nullptr);
	vkDestroyDescriptorSetLayout(Framework.GetWindow().GetContext().GetDevice(), _objectDescriptorSetLayout, nullptr);

	vkDestroyDescriptorPool(Framework.GetWindow().GetContext().GetDevice(), _descriptorPool, nullptr);
	KT_DEBUG_LOG("cleaned up shader");
}

VkPipeline KtShader::GetGraphicsPipeline() const
{
	return _graphicsPipeline;
}

VkPipelineLayout KtShader::GetPipelineLayout() const
{
	return _pipelineLayout;
}

void KtShader::CreateImageTexture()
{
	_imageTexture = new KtImageTexture(R"(C:\Users\nicos\Documents\Visual Studio 2022\Projects\KotonoEngine\assets\models\viking_room.png)");
}

void KtShader::CreateDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> set0Bindings = { uboLayoutBinding, samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo set0LayoutInfo{};
	set0LayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	set0LayoutInfo.bindingCount = static_cast<uint32_t>(set0Bindings.size());
	set0LayoutInfo.pBindings = set0Bindings.data();

	VK_CHECK_THROW(
		vkCreateDescriptorSetLayout(Framework.GetWindow().GetContext().GetDevice(), &set0LayoutInfo, nullptr, &_uniformDescriptorSetLayout),
		"failed to create descriptor set layout!"
	);


	VkDescriptorSetLayoutBinding objectBufferLayoutBinding{};
	objectBufferLayoutBinding.binding = 0;
	objectBufferLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	objectBufferLayoutBinding.descriptorCount = 1;
	objectBufferLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	objectBufferLayoutBinding.pImmutableSamplers = nullptr; // Optional
	
	std::array<VkDescriptorSetLayoutBinding, 1> set1Bindings = { objectBufferLayoutBinding };
	VkDescriptorSetLayoutCreateInfo set1LayoutInfo{};
	set1LayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	set1LayoutInfo.bindingCount = static_cast<uint32_t>(set1Bindings.size());
	set1LayoutInfo.pBindings = set1Bindings.data();

	VK_CHECK_THROW(
		vkCreateDescriptorSetLayout(Framework.GetWindow().GetContext().GetDevice(), &set1LayoutInfo, nullptr, &_objectDescriptorSetLayout),
		"failed to create descriptor set layout!"
	);
}

void KtShader::CreateDescriptorPool()
{
	std::array<VkDescriptorPoolSize, 3> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // View projection buffer
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // Image texture
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	poolSizes[2].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; // Object data buffer
    poolSizes[2].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT); 

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) * 2; // One set for global, one for object buffer

    VK_CHECK_THROW(
        vkCreateDescriptorPool(Framework.GetWindow().GetContext().GetDevice(), &poolInfo, nullptr, &_descriptorPool),
        "failed to create descriptor pool!"
    );
}

void KtShader::CreateDescriptorSets()
{
	std::array<VkDescriptorSetLayout, MAX_FRAMES_IN_FLIGHT> globalLayouts{};
	globalLayouts.fill(_uniformDescriptorSetLayout);

	std::array<VkDescriptorSetLayout, MAX_FRAMES_IN_FLIGHT> objectLayouts{};
	objectLayouts.fill(_objectDescriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(globalLayouts.size());
	allocInfo.pSetLayouts = globalLayouts.data();

	VK_CHECK_THROW(
		vkAllocateDescriptorSets(Framework.GetWindow().GetContext().GetDevice(), &allocInfo, _uniformDescriptorSets.data()),
		"failed to allocate global descriptor sets!"
	);

	allocInfo.descriptorSetCount = static_cast<uint32_t>(objectLayouts.size());
	allocInfo.pSetLayouts = objectLayouts.data();
	VK_CHECK_THROW(
		vkAllocateDescriptorSets(Framework.GetWindow().GetContext().GetDevice(), &allocInfo, _objectDescriptorSets.data()),
		"failed to allocate object descriptor sets!"
	);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		UpdateDescriptorSet(static_cast<uint32_t>(i), _imageTexture);
	}
}

void KtShader::UpdateDescriptorSet(const uint32_t imageIndex, const KtImageTexture* imageTexture)
{
	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.buffer = _uniformBuffers[imageIndex].Buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(ViewProjectionBuffer);

	VkDescriptorImageInfo imageInfo{};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = imageTexture->ImageView;
	imageInfo.sampler = imageTexture->Sampler;

	VkDescriptorBufferInfo objectBufferInfo{};
	objectBufferInfo.buffer = _objectBuffers[imageIndex].Buffer;
	objectBufferInfo.offset = 0;
	objectBufferInfo.range = GetObjectBufferSize(imageIndex);

	std::array<VkWriteDescriptorSet, 3> descriptorWrites{};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _uniformDescriptorSets[imageIndex];
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = _uniformDescriptorSets[imageIndex];
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &imageInfo;

	descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[2].dstSet = _objectDescriptorSets[imageIndex]; // Set 1 (Object Data)
	descriptorWrites[2].dstBinding = 0;
	descriptorWrites[2].dstArrayElement = 0;
	descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptorWrites[2].descriptorCount = 1;
	descriptorWrites[2].pBufferInfo = &objectBufferInfo;

	vkUpdateDescriptorSets(Framework.GetWindow().GetContext().GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void KtShader::CreateUniformBuffers()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		CreateUniformBuffer(static_cast<uint32_t>(i));
	}
}

void KtShader::CreateObjectBuffers()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		CreateObjectBuffer(static_cast<uint32_t>(i));
	}
}

void KtShader::CreateUniformBuffer(const uint32_t imageIndex)
{
	Framework.GetWindow().GetContext().CreateBuffer(
		sizeof(ViewProjectionBuffer),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		0,
		_uniformBuffers[imageIndex],
		VMA_MEMORY_USAGE_GPU_ONLY
	);

	Framework.GetWindow().GetContext().CreateBuffer(
		sizeof(ViewProjectionBuffer),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VMA_ALLOCATION_CREATE_MAPPED_BIT,
		_stagingUniformBuffers[imageIndex],
		VMA_MEMORY_USAGE_CPU_TO_GPU
	);
}

void KtShader::CreateObjectBuffer(const uint32_t imageIndex)
{
	KT_DEBUG_LOG("0bject buffer size at frame %u: %llu", imageIndex, GetObjectBufferSize(imageIndex));
	Framework.GetWindow().GetContext().CreateBuffer(
		GetObjectBufferSize(imageIndex),
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, // Can be used as SSBO & can receive data from staging
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, // Optimized for GPU access
		0, // No need for MAPPED_BIT since we won’t access this from CPU
		_objectBuffers[imageIndex],
		VMA_MEMORY_USAGE_GPU_ONLY // Best for performance
	);

	Framework.GetWindow().GetContext().CreateBuffer(
		GetObjectBufferSize(imageIndex),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, // Usage flags
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // Memory properties
		VMA_ALLOCATION_CREATE_MAPPED_BIT, // Allocation flags
		_stagingObjectBuffers[imageIndex],
		VMA_MEMORY_USAGE_CPU_TO_GPU
	);
}

void KtShader::UpdateUniformBuffer(const uint32_t imageIndex)
{
	const auto swapChainExtent = Framework.GetWindow().GetRenderer().GetSwapChainExtent();

	ViewProjectionBuffer ubo{};
	ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.Projection = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 100.0f);
	ubo.Projection[1][1] *= -1.0f;

	memcpy(_stagingUniformBuffers[imageIndex].AllocationInfo.pMappedData, &ubo, sizeof(ViewProjectionBuffer));

	Framework.GetWindow().GetContext().CopyBuffer(
		_stagingUniformBuffers[imageIndex].Buffer,
		_uniformBuffers[imageIndex].Buffer,
		sizeof(ViewProjectionBuffer)
	);
}

void KtShader::UpdateObjectBuffer(const std::vector<KtObjectData3D>& objectDatas, const uint32_t imageIndex)
{
	// Ensure buffer sizes are enough
	SetObjectCount(objectDatas.size(), imageIndex);

	// Copy data to the staging buffer
	memcpy(_stagingObjectBuffers[imageIndex].AllocationInfo.pMappedData, objectDatas.data(), GetObjectBufferSize(imageIndex));

	Framework.GetWindow().GetContext().CopyBuffer(
		_stagingObjectBuffers[imageIndex].Buffer,
		_objectBuffers[imageIndex].Buffer,
		GetObjectBufferSize(imageIndex)
	);
}

void KtShader::CmdBind(VkCommandBuffer commandBuffer) const
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);
}

void KtShader::CmdBindDescriptorSets(VkCommandBuffer commandBuffer, const uint32_t imageIndex)
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

void KtShader::SetObjectCount(const VkDeviceSize objectCount, const uint32_t imageIndex)
{
	if (_objectCounts[imageIndex] != objectCount)
	{
		_objectCounts[imageIndex] = objectCount;
		KT_DEBUG_LOG("Object count at frame %u: %llu", imageIndex, objectCount);

		vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), _objectBuffers[imageIndex].Buffer, _objectBuffers[imageIndex].Allocation);
		vmaDestroyBuffer(Framework.GetWindow().GetContext().GetAllocator(), _stagingObjectBuffers[imageIndex].Buffer, _stagingObjectBuffers[imageIndex].Allocation);
		CreateObjectBuffer(imageIndex);

		UpdateDescriptorSet(imageIndex, _imageTexture);
	}
}

const VkDeviceSize KtShader::GetObjectBufferSize(const uint32_t imageIndex) const
{
	return sizeof(KtObjectData3D) * (_objectCounts[imageIndex] == 0 ? 1 : _objectCounts[imageIndex]);
}

void KtShader::CreateGraphicsPipeline()
{
	const std::vector<char> vertShaderCode = KtFile(_vertPath).GetBinaryContent();
	const std::vector<char> fragShaderCode = KtFile(_fragPath).GetBinaryContent();

	VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

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

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	VkVertexInputBindingDescription bindingDescriptions[] =
	{ 
		// Binding for vertex data
		{ 0, sizeof(KtVertex), VK_VERTEX_INPUT_RATE_VERTEX }
	};
	VkVertexInputAttributeDescription attributeDescriptions[] =
	{
		// Vertex attributes
		{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(KtVertex, Position) },
		{ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(KtVertex, Color) },
		{ 2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(KtVertex, TexCoord) }
	};

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions;
	vertexInputInfo.vertexAttributeDescriptionCount = 3;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)Framework.GetWindow().GetRenderer().GetSwapChainExtent().width;
	viewport.height = (float)Framework.GetWindow().GetRenderer().GetSwapChainExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = Framework.GetWindow().GetRenderer().GetSwapChainExtent();

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f; // For rasterizer.polygonMode = VK_POLYGON_MODE_LINE; 
	// enable wideLines GPU feature for thicker
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_TRUE; // enable sample shading in the pipeline
	multisampling.rasterizationSamples = Framework.GetWindow().GetContext().GetMSAASamples();
	multisampling.minSampleShading = 0.2f; // min fraction for sample shading; closer to one is smoother
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

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
	
	std::array<VkDescriptorSetLayout, 2> setLayouts = { _uniformDescriptorSetLayout, _objectDescriptorSetLayout };
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());;
	pipelineLayoutInfo.pSetLayouts = setLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(Framework.GetWindow().GetContext().GetDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = _pipelineLayout;
	pipelineInfo.renderPass = Framework.GetWindow().GetRenderer().GetRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	if (vkCreateGraphicsPipelines(Framework.GetWindow().GetContext().GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	vkDestroyShaderModule(Framework.GetWindow().GetContext().GetDevice(), fragShaderModule, nullptr);
	vkDestroyShaderModule(Framework.GetWindow().GetContext().GetDevice(), vertShaderModule, nullptr);
}

const VkShaderModule KtShader::CreateShaderModule(const std::vector<char>& code) const
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(Framework.GetWindow().GetContext().GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}