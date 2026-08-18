#include "PipelineResourceManager.h"

#include "PushConstants.h"
#include <array>
#include <assert.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>

static constexpr u32 MAX_TEXTURES{ 65536 };
static constexpr u32 MAX_TEXTURE_ARRAYS{ 1024 };
static constexpr u32 MAX_SAMPLERS{ 4096 };
static constexpr u32 MAX_SHADOW_SAMPLERS{ 1024 };
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

	texturePool_.maxSlots = MAX_TEXTURES;
	textureArrayPool_.maxSlots = MAX_TEXTURE_ARRAYS;
	samplerPool_.maxSlots = MAX_SAMPLERS;
	shadowSamplerPool_.maxSlots = MAX_SHADOW_SAMPLERS;
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
	const u32 slot{ AllocateSlot(texturePool_) };

	WriteDescriptorSet(VK_NULL_HANDLE, imageView
		, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		, 0
		, slot
		, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
	);

	return slot;
}

u32 GPipelineResourceManager::RegisterTextureArray(VkImageView imageView)
{
	const u32 slot{ AllocateSlot(textureArrayPool_) };

	WriteDescriptorSet(VK_NULL_HANDLE, imageView
		, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		, 1
		, slot
		, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
	);

	return slot;
}

u32 GPipelineResourceManager::RegisterSampler(VkSampler sampler)
{
	const u32 slot{ AllocateSlot(samplerPool_) };

	WriteDescriptorSet(sampler, VK_NULL_HANDLE
		, VK_IMAGE_LAYOUT_UNDEFINED
		, 2
		, slot
		, VK_DESCRIPTOR_TYPE_SAMPLER
	);

	return slot;
}

u32 GPipelineResourceManager::RegisterShadowSampler(VkSampler sampler)
{
	const u32 slot{ AllocateSlot(shadowSamplerPool_) };

	WriteDescriptorSet(sampler, VK_NULL_HANDLE
		, VK_IMAGE_LAYOUT_UNDEFINED
		, 3
		, slot
		, VK_DESCRIPTOR_TYPE_SAMPLER
	);

	return slot;
}

void GPipelineResourceManager::UnregisterTexture(const u32 slot)
{
	texturePool_.freeSlots.push_back(slot);
}

void GPipelineResourceManager::UnregisterTextureArray(const u32 slot)
{
	textureArrayPool_.freeSlots.push_back(slot);
}

void GPipelineResourceManager::UnregisterSampler(const u32 slot)
{
	samplerPool_.freeSlots.push_back(slot);
}

void GPipelineResourceManager::UnregisterShadowSampler(const u32 slot)
{
	shadowSamplerPool_.freeSlots.push_back(slot);
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
	constexpr std::array<VkDescriptorSetLayoutBinding, 5> BINDINGS{ {
		{ 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,	MAX_TEXTURES,			VK_SHADER_STAGE_ALL, nullptr },
		{ 1, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,	MAX_TEXTURE_ARRAYS,		VK_SHADER_STAGE_ALL, nullptr },
		{ 2, VK_DESCRIPTOR_TYPE_SAMPLER,		MAX_SAMPLERS,			VK_SHADER_STAGE_ALL, nullptr },
		{ 3, VK_DESCRIPTOR_TYPE_SAMPLER,		MAX_SHADOW_SAMPLERS,	VK_SHADER_STAGE_ALL, nullptr },
		{ 4, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,	MAX_STORAGE_IMAGES,		VK_SHADER_STAGE_ALL, nullptr },
	} };

	// Add VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT to the last binding (only) to make it resizeable
	constexpr std::array BINDING_FLAGS{
		BINDLESS_FLAGS,
		BINDLESS_FLAGS,
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
	constexpr std::array<VkDescriptorPoolSize, 5> POOL_SIZES{ {
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,	MAX_TEXTURES		},
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,	MAX_TEXTURE_ARRAYS	},
		{ VK_DESCRIPTOR_TYPE_SAMPLER,		MAX_SAMPLERS		},
		{ VK_DESCRIPTOR_TYPE_SAMPLER,		MAX_SHADOW_SAMPLERS	},
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

u32 GPipelineResourceManager::AllocateSlot(ResourcePool& resourcePool) const
{
	if (!resourcePool.freeSlots.empty())
	{
		const u32 slot{ resourcePool.freeSlots.back() };
		resourcePool.freeSlots.pop_back();
		return slot;
	}
	assert(resourcePool.nextSlot < resourcePool.maxSlots);
	return resourcePool.nextSlot++;
}

void GPipelineResourceManager::WriteDescriptorSet(VkSampler sampler, VkImageView imageView
	, const VkImageLayout imageLayout
	, const u32 binding
	, const u32 slot
	, const VkDescriptorType descriptorType) const
{
	const VkDescriptorImageInfo imageInfo{
		.sampler = sampler,
		.imageView = imageView,
		.imageLayout = imageLayout,
	};
	const VkWriteDescriptorSet write{
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = descriptorSet_,
		.dstBinding = binding,
		.dstArrayElement = slot,
		.descriptorCount = 1,
		.descriptorType = descriptorType,
		.pImageInfo = &imageInfo,
	};

	// This is safe mid-frame as long as the slot isn't in active use by in-flight command buffers
	vkUpdateDescriptorSets(Context.GetDevice(), 1, &write, 0, nullptr);
}
