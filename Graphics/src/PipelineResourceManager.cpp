#include "PipelineResourceManager.h"
#include "frames_in_flight.h"
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>

#include <glm/glm.hpp>

static constexpr u32 MAX_TEXTURES{ 65536 };
static constexpr u32 MAX_SAMPLERS{ 4096 };
static constexpr u32 MAX_STORAGE_IMAGES{ 1024 };

static constexpr VkDescriptorBindingFlags BINDLESS_FLAGS{
	VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
	VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT
};

void SPipelineResourceManager::Init()
{
	CreateGlobalDescriptorSetLayout();
	CreateUniformDescriptorSetLayout();
	CreatePipelineLayout();
	CreateDescriptorPool();
	CreateGlobalDescriptorSet();
	CreateFrameDataBuffers();
}

void SPipelineResourceManager::Cleanup() const
{
	for (auto& frameData : frameDatas_)
	{
		vmaDestroyBuffer(Context.GetAllocator(), frameData.ubo, frameData.uboAllocation);
	}
	vkDestroyDescriptorPool(Context.GetDevice(), descriptorPool_, nullptr);
	vkDestroyPipelineLayout(Context.GetDevice(), pipelineLayout_, nullptr);
	vkDestroyDescriptorSetLayout(Context.GetDevice(), uniformDescriptorSetLayout_, nullptr);
	vkDestroyDescriptorSetLayout(Context.GetDevice(), globalDescriptorSetLayout_, nullptr);
}

VkDescriptorPool SPipelineResourceManager::GetDescriptorPool() const
{
	return descriptorPool_;
}

VkPipelineLayout SPipelineResourceManager::GetPipelineLayout() const
{
	return pipelineLayout_;
}

VkDescriptorSet SPipelineResourceManager::GetGlobalDescriptorSet() const
{
	return globalDescriptorSet_;
}

void SPipelineResourceManager::SetFrameUBO(const FrameUBO& frameUBO)
{
	frameUBO_ = frameUBO;
}

void SPipelineResourceManager::UpdateMappedFrameUBO(const u32 frameIndex) const
{
	*frameDatas_[frameIndex].uboMapped = frameUBO_;
}

u32 SPipelineResourceManager::RegisterTexture(VkImageView imageView)
{
	const u32 slot{ AllocateTextureSlot() };

	const VkDescriptorImageInfo imageInfo{
		.sampler = VK_NULL_HANDLE,
		.imageView = imageView,
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
	};
	const VkWriteDescriptorSet write{
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = globalDescriptorSet_,
		.dstBinding = 0,
		.dstArrayElement = slot,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
		.pImageInfo = &imageInfo,
	};

	// This is safe mid-frame as long as the slot isn't in active use by in-flight command buffers
	vkUpdateDescriptorSets(Context.GetDevice(), 1, &write, 0, nullptr);

	return slot;
}

u32 SPipelineResourceManager::RegisterSampler(VkSampler sampler)
{
	const u32 slot{ AllocateSamplerSlot() };

	const VkDescriptorImageInfo samplerInfo{
		.sampler = sampler,
		.imageView = VK_NULL_HANDLE,
		.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
	};
	const VkWriteDescriptorSet write{
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = globalDescriptorSet_,
		.dstBinding = 1,
		.dstArrayElement = slot,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
		.pImageInfo = &samplerInfo,
	};

	// This is safe mid-frame as long as the slot isn't in active use by in-flight command buffers
	vkUpdateDescriptorSets(Context.GetDevice(), 1, &write, 0, nullptr);

	return slot;
}

void SPipelineResourceManager::UnregisterTexture(const u32 slot)
{
	freeTextureSlots_.push_back(slot);
}

void SPipelineResourceManager::UnregisterSampler(const u32 slot)
{
	freeSamplerSlots_.push_back(slot);
}

void SPipelineResourceManager::CmdBindGlobalDescriptorSet(VkCommandBuffer commandBuffer) const
{
	vkCmdBindDescriptorSets(commandBuffer
		, VK_PIPELINE_BIND_POINT_GRAPHICS
		, pipelineLayout_
		, 0
		, 1
		, &globalDescriptorSet_
		, 0
		, nullptr
	);
}

void SPipelineResourceManager::CmdPushUniformDescriptorSet(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	const VkDescriptorBufferInfo bufInfo{
		.buffer = frameDatas_[frameIndex].ubo,
		.offset = 0,
		.range = sizeof(FrameUBO),
	};
	const VkWriteDescriptorSet frameUBOWrite{
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstBinding = 0,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.pBufferInfo = &bufInfo,
	};
	vkCmdPushDescriptorSet(commandBuffer
		, VK_PIPELINE_BIND_POINT_GRAPHICS
		, pipelineLayout_
		, 1
		, 1
		, &frameUBOWrite
	);
}

void SPipelineResourceManager::CreateGlobalDescriptorSetLayout()
{
	constexpr std::array<VkDescriptorSetLayoutBinding, 3> BINDINGS{ {
		{ 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,	MAX_TEXTURES,		VK_SHADER_STAGE_ALL, nullptr },
		{ 1, VK_DESCRIPTOR_TYPE_SAMPLER,		MAX_SAMPLERS,		VK_SHADER_STAGE_ALL, nullptr },
		{ 2, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,	MAX_STORAGE_IMAGES,	VK_SHADER_STAGE_ALL, nullptr },
	} };

	/// Add VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT to the last binding (only) to make it resizeable
	constexpr std::array BINDING_FLAGS{
		BINDLESS_FLAGS,
		BINDLESS_FLAGS,
		BINDLESS_FLAGS,
	};

	const VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
		.bindingCount = static_cast<u32>(BINDING_FLAGS.size()),
		.pBindingFlags = BINDING_FLAGS.data(),
	};

	const VkDescriptorSetLayoutCreateInfo layoutInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext = &bindingFlagsInfo,
		.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT,
		.bindingCount = static_cast<u32>(BINDINGS.size()),
		.pBindings = BINDINGS.data(),
	};

	VK_CHECK_THROW(
		vkCreateDescriptorSetLayout(Context.GetDevice(), &layoutInfo, nullptr, &globalDescriptorSetLayout_),
		"failed to create descriptor set layout!"
	);
}

void SPipelineResourceManager::CreateUniformDescriptorSetLayout()
{
	const VkDescriptorSetLayoutBinding uniformBinding{
		.binding = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_ALL,
	};

	const VkDescriptorSetLayoutCreateInfo uniformLayoutInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR,
		.bindingCount = 1,
		.pBindings = &uniformBinding,
	};

	vkCreateDescriptorSetLayout(Context.GetDevice(), &uniformLayoutInfo, nullptr, &uniformDescriptorSetLayout_);
}

void SPipelineResourceManager::CreatePipelineLayout()
{
	const VkPushConstantRange pushConstantRange{
	   .stageFlags = VK_SHADER_STAGE_ALL,
	   .offset = 0,
	   .size = sizeof(UPushConstants),
	};

	const std::array setLayouts{ globalDescriptorSetLayout_, uniformDescriptorSetLayout_ };

	const std::array pushConstantRanges{ pushConstantRange };

	const VkPipelineLayoutCreateInfo pipelineLayoutInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,

		.setLayoutCount = static_cast<u32>(setLayouts.size()),
		.pSetLayouts = setLayouts.data(),

		.pushConstantRangeCount = static_cast<u32>(pushConstantRanges.size()),
		.pPushConstantRanges = pushConstantRanges.data(),
	};

	VK_CHECK_THROW(
		vkCreatePipelineLayout(Context.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout_),
		"failed to create pipeline layout!"
	);
}

void SPipelineResourceManager::CreateDescriptorPool()
{
	constexpr std::array<VkDescriptorPoolSize, 3> POOL_SIZES{ {
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,	MAX_TEXTURES		},
		{ VK_DESCRIPTOR_TYPE_SAMPLER,		MAX_SAMPLERS		},
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,	MAX_STORAGE_IMAGES	},
	} };

	const VkDescriptorPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT,
		.maxSets = 1,
		.poolSizeCount = static_cast<u32>(POOL_SIZES.size()),
		.pPoolSizes = POOL_SIZES.data(),
	};

	VK_CHECK_THROW(
		vkCreateDescriptorPool(Context.GetDevice(), &poolInfo, nullptr, &descriptorPool_),
		"failed to create descriptor pool!"
	);
}

void SPipelineResourceManager::CreateGlobalDescriptorSet()
{
	const VkDescriptorSetAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = descriptorPool_,
		.descriptorSetCount = 1,
		.pSetLayouts = &globalDescriptorSetLayout_,
	};
	vkAllocateDescriptorSets(Context.GetDevice(), &allocInfo, &globalDescriptorSet_);
}

void SPipelineResourceManager::CreateFrameDataBuffers()
{
	for (auto& frameData : frameDatas_)
	{
		const VmaAllocationCreateInfo allocCreateInfo{
			.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT 
				| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
			.usage = VMA_MEMORY_USAGE_AUTO,
		};
		const VkBufferCreateInfo bufCreateInfo{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = sizeof(FrameUBO),
			.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		};

		VmaAllocationInfo allocInfo;
		vmaCreateBuffer(Context.GetAllocator()
			, &bufCreateInfo
			, &allocCreateInfo
			, &frameData.ubo
			, &frameData.uboAllocation
			, &allocInfo
		);

		frameData.uboMapped = static_cast<FrameUBO*>(allocInfo.pMappedData);
	}
}

u32 SPipelineResourceManager::AllocateTextureSlot()
{
	if (!freeTextureSlots_.empty())
	{
		const u32 slot{ freeTextureSlots_.back() };
		freeTextureSlots_.pop_back();
		return slot;
	}
	assert(nextTextureSlot_ < MAX_TEXTURES);
	return nextTextureSlot_++;
}

u32 SPipelineResourceManager::AllocateSamplerSlot()
{
	if (!freeSamplerSlots_.empty())
	{
		const u32 slot{ freeSamplerSlots_.back() };
		freeSamplerSlots_.pop_back();
		return slot;
	}
	assert(nextSamplerSlot_ < MAX_SAMPLERS);
	return nextSamplerSlot_++;
}
