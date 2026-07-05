#include "PipelineResourceManager.h"
#include <array>
#include <assert.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>

static constexpr u32 MAX_TEXTURES{ 65536 };
static constexpr u32 MAX_SAMPLERS{ 4096 };
static constexpr u32 MAX_STORAGE_IMAGES{ 1024 };

static constexpr VkDescriptorBindingFlags BINDLESS_FLAGS{
	VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
	VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT
};

void GPipelineResourceManager::Init()
{
	CreateDescriptorSetLayout();
	CreatePipelineLayout();
	CreateDescriptorPool();
	CreateDescriptorSet();
}

void GPipelineResourceManager::Cleanup() const
{
	vkDestroyDescriptorPool(Context.GetDevice(), descriptorPool_, nullptr);
	vkDestroyPipelineLayout(Context.GetDevice(), pipelineLayout_, nullptr);
	vkDestroyDescriptorSetLayout(Context.GetDevice(), descriptorSetLayout_, nullptr);
}

VkDescriptorPool GPipelineResourceManager::GetDescriptorPool() const
{
	return descriptorPool_;
}

VkPipelineLayout GPipelineResourceManager::GetPipelineLayout() const
{
	return pipelineLayout_;
}

VkDescriptorSet GPipelineResourceManager::GetDescriptorSet() const
{
	return descriptorSet_;
}

u32 GPipelineResourceManager::RegisterTexture(VkImageView imageView)
{
	const u32 slot{ AllocateTextureSlot() };

	const VkDescriptorImageInfo imageInfo{
		.sampler = VK_NULL_HANDLE,
		.imageView = imageView,
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
	};
	const VkWriteDescriptorSet write{
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = descriptorSet_,
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

u32 GPipelineResourceManager::RegisterSampler(VkSampler sampler)
{
	const u32 slot{ AllocateSamplerSlot() };

	const VkDescriptorImageInfo samplerInfo{
		.sampler = sampler,
		.imageView = VK_NULL_HANDLE,
		.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
	};
	const VkWriteDescriptorSet write{
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = descriptorSet_,
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

void GPipelineResourceManager::UnregisterTexture(const u32 slot)
{
	freeTextureSlots_.push_back(slot);
}

void GPipelineResourceManager::UnregisterSampler(const u32 slot)
{
	freeSamplerSlots_.push_back(slot);
}

void GPipelineResourceManager::CmdBindDescriptorSet(VkCommandBuffer commandBuffer) const
{
	vkCmdBindDescriptorSets(commandBuffer
		, VK_PIPELINE_BIND_POINT_GRAPHICS
		, pipelineLayout_
		, 0
		, 1
		, &descriptorSet_
		, 0
		, nullptr
	);
}

void GPipelineResourceManager::CreateDescriptorSetLayout()
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
		vkCreateDescriptorSetLayout(Context.GetDevice(), &layoutInfo, nullptr, &descriptorSetLayout_),
		"failed to create descriptor set layout!"
	);
}

void GPipelineResourceManager::CreatePipelineLayout()
{
	const VkPushConstantRange pushConstantRange{
	   .stageFlags = VK_SHADER_STAGE_ALL,
	   .offset = 0,
	   .size = sizeof(UPushConstants),
	};

	const std::array setLayouts{ descriptorSetLayout_ };
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

void GPipelineResourceManager::CreateDescriptorPool()
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

void GPipelineResourceManager::CreateDescriptorSet()
{
	const VkDescriptorSetAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = descriptorPool_,
		.descriptorSetCount = 1,
		.pSetLayouts = &descriptorSetLayout_,
	};
	vkAllocateDescriptorSets(Context.GetDevice(), &allocInfo, &descriptorSet_);
}

u32 GPipelineResourceManager::AllocateTextureSlot()
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

u32 GPipelineResourceManager::AllocateSamplerSlot()
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
