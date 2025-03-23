#include "Shader3D.h"
#include <vulkan/vulkan.h>
#include <span>
#include "Vertex3D.h"
#include "File.h"
#include "Framework.h"
#include "log.h"
#include "vk_utils.h"

void KtShader3D::Init()
{
	SetVertPath(R"(C:\Users\nicos\Documents\Visual Studio 2022\Projects\KotonoEngine\KotonoFramework\shaders\shader3DVert.spv)");
	SetFragPath(R"(C:\Users\nicos\Documents\Visual Studio 2022\Projects\KotonoEngine\KotonoFramework\shaders\shader3DFrag.spv)");
	CreateDescriptorSetLayouts();
	CreateDescriptorPools();
	CreateImageTexture();
	CreateUniformBuffers();
	CreateObjectBuffers();
	CreateDescriptorSets();
	CreateGraphicsPipelines();
}

void KtShader3D::Cleanup()
{
	KT_DEBUG_LOG("cleaning up shader");

	KtShader::Cleanup();

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

void KtShader3D::CreateImageTexture()
{
	_imageTexture = Framework.GetImageTextureManager().Get(
		R"(C:\Users\nicos\Documents\Visual Studio 2022\Projects\KotonoEngine\assets\models\viking_room.png)"
	);
}

void KtShader3D::CreateDescriptorSetLayouts()
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
	CreateDescriptorSetLayout(_uniformDescriptorSetLayout, set0Bindings);

	VkDescriptorSetLayoutBinding objectBufferLayoutBinding{};
	objectBufferLayoutBinding.binding = 0;
	objectBufferLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	objectBufferLayoutBinding.descriptorCount = 1;
	objectBufferLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	objectBufferLayoutBinding.pImmutableSamplers = nullptr; // Optional
	
	std::array<VkDescriptorSetLayoutBinding, 1> set1Bindings = { objectBufferLayoutBinding };
	CreateDescriptorSetLayout(_objectDescriptorSetLayout, set1Bindings);
}

void KtShader3D::CreateDescriptorPools()
{
	std::array<VkDescriptorPoolSize, 3> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // View projection buffer
    poolSizes[0].descriptorCount = static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // Image texture
    poolSizes[1].descriptorCount = static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT);
	poolSizes[2].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; // Object data buffer
    poolSizes[2].descriptorCount = static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT); 

	CreateDescriptorPool(poolSizes, 2);
}

void KtShader3D::UpdateDescriptorSet(const uint32_t imageIndex)
{
	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.buffer = _uniformBuffers[imageIndex].Buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(KtUniformData3D);

	VkDescriptorBufferInfo objectBufferInfo{};
	objectBufferInfo.buffer = _objectBuffers[imageIndex].Buffer;
	objectBufferInfo.offset = 0;
	objectBufferInfo.range = GetObjectBufferCount(imageIndex) * sizeof(KtObjectData2D);

	VkDescriptorImageInfo imageInfo = _imageTexture->GetDescriptorImageInfo();

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

	vkUpdateDescriptorSets(Framework.GetContext().GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void KtShader3D::CreateUniformBuffers()
{
	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		CreateUniformBuffer(static_cast<uint32_t>(i), sizeof(KtUniformData3D));
	}
}

void KtShader3D::CreateObjectBuffers()
{
	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		CreateObjectBuffer(static_cast<uint32_t>(i), GetObjectBufferCount(static_cast<uint32_t>(i)) * sizeof(KtObjectData2D));
	}
}

void KtShader3D::UpdateUniformBuffer(const KtUniformData3D& uniformData, const uint32_t imageIndex)
{
	memcpy(_stagingUniformBuffers[imageIndex].AllocationInfo.pMappedData, &uniformData, sizeof(KtUniformData3D));

	Framework.GetContext().CopyBuffer(
		_stagingUniformBuffers[imageIndex].Buffer,
		_uniformBuffers[imageIndex].Buffer,
		sizeof(KtUniformData3D)
	);
}

void KtShader3D::UpdateObjectBuffer(const std::span<KtObjectData3D> objectDatas, const uint32_t imageIndex)
{
	// Ensure buffer sizes are enough
	SetObjectCount(objectDatas.size(), imageIndex);

	// Copy data to the staging buffer
	memcpy(_stagingObjectBuffers[imageIndex].AllocationInfo.pMappedData, objectDatas.data(), GetObjectBufferCount(imageIndex) * sizeof(KtObjectData2D));

	Framework.GetContext().CopyBuffer(
		_stagingObjectBuffers[imageIndex].Buffer,
		_objectBuffers[imageIndex].Buffer,
		GetObjectBufferCount(imageIndex) * sizeof(KtObjectData2D)
	);
}

void KtShader3D::CreateGraphicsPipelines()
{
	std::array<VkVertexInputBindingDescription, 1> bindingDescriptions =
	{ 
		VkVertexInputBindingDescription { 0, sizeof(KtVertex3D), VK_VERTEX_INPUT_RATE_VERTEX }
	};
	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions =
	{
		VkVertexInputAttributeDescription { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(KtVertex3D, Position) },
		VkVertexInputAttributeDescription { 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(KtVertex3D, Color) },
		VkVertexInputAttributeDescription { 2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(KtVertex3D, TexCoord) }
	};

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
	multisampling.rasterizationSamples = Framework.GetContext().GetMSAASamples();
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
	
	std::array<VkDescriptorSetLayout, 2> setLayouts = { _uniformDescriptorSetLayout, _objectDescriptorSetLayout };

	CreateGraphicsPipeline(bindingDescriptions, attributeDescriptions, rasterizer, multisampling, depthStencil, colorBlendAttachment, setLayouts);
}
