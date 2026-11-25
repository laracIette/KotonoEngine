#include "PipelineResourceManager/PipelineResourceManager.h"

#include "Containers/Array.h"
#include "PushConstants.h"
#include "check.h"
#include "Device/Device.h"
#include "vk_utils/vk_utils.h"

static constexpr u32 MAX_TEXTURES{ 65536 };
static constexpr u32 MAX_TEXTURE_ARRAYS{ 1024 };
static constexpr u32 MAX_SAMPLERS{ 4096 };
static constexpr u32 MAX_SHADOW_SAMPLERS{ 1024 };
static constexpr u32 MAX_STORAGE_IMAGES{ 1024 };

static constexpr VkDescriptorBindingFlags BINDLESS_FLAGS{
	VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
	VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT
};

UPipelineResourceManager::UPipelineResourceManager(UDevice& device)
	: device_{ device }
	, texturePool_{}
	, textureArrayPool_{}
	, samplerPool_{}
	, shadowSamplerPool_{}
{
}

void UPipelineResourceManager::Init()
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

void UPipelineResourceManager::Cleanup() const
{
	vkDestroyDescriptorPool(device_.GetDevice(), descriptorPool_, nullptr);
	vkDestroyPipelineLayout(device_.GetDevice(), pipelineLayout_, nullptr);
	vkDestroyDescriptorSetLayout(device_.GetDevice(), descriptorSetLayout_, nullptr);
}

VkDescriptorPool UPipelineResourceManager::GetDescriptorPool() const
{
	return descriptorPool_;
}

VkPipelineLayout UPipelineResourceManager::GetPipelineLayout() const
{
	return pipelineLayout_;
}

VkDescriptorSet UPipelineResourceManager::GetDescriptorSet() const
{
	return descriptorSet_;
}

u32 UPipelineResourceManager::RegisterTexture(VkImageView imageView)
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

u32 UPipelineResourceManager::RegisterTextureArray(VkImageView imageView)
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

u32 UPipelineResourceManager::RegisterSampler(VkSampler sampler)
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

u32 UPipelineResourceManager::RegisterShadowSampler(VkSampler sampler)
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

void UPipelineResourceManager::UnregisterTexture(u32 slot)
{
	texturePool_.freeSlots.Add(slot);
}

void UPipelineResourceManager::UnregisterTextureArray(u32 slot)
{
	textureArrayPool_.freeSlots.Add(slot);
}

void UPipelineResourceManager::UnregisterSampler(u32 slot)
{
	samplerPool_.freeSlots.Add(slot);
}

void UPipelineResourceManager::UnregisterShadowSampler(u32 slot)
{
	shadowSamplerPool_.freeSlots.Add(slot);
}

void UPipelineResourceManager::CmdBindDescriptorSet(VkCommandBuffer commandBuffer) const
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

void UPipelineResourceManager::CreateDescriptorSetLayout()
{
	constexpr UArray BINDINGS{
		VkDescriptorSetLayoutBinding{ 0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,	MAX_TEXTURES,			VK_SHADER_STAGE_ALL, nullptr },
		VkDescriptorSetLayoutBinding{ 1, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,	MAX_TEXTURE_ARRAYS,	VK_SHADER_STAGE_ALL, nullptr },
		VkDescriptorSetLayoutBinding{ 2, VK_DESCRIPTOR_TYPE_SAMPLER,		    MAX_SAMPLERS,			VK_SHADER_STAGE_ALL, nullptr },
		VkDescriptorSetLayoutBinding{ 3, VK_DESCRIPTOR_TYPE_SAMPLER,		    MAX_SHADOW_SAMPLERS,	VK_SHADER_STAGE_ALL, nullptr },
		VkDescriptorSetLayoutBinding{ 4, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,	MAX_STORAGE_IMAGES,	VK_SHADER_STAGE_ALL, nullptr },
	};

	// Add VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT to the last binding (only) to make it resizeable
	constexpr UArray BINDING_FLAGS{
		BINDLESS_FLAGS,
		BINDLESS_FLAGS,
		BINDLESS_FLAGS,
		BINDLESS_FLAGS,
		BINDLESS_FLAGS,
	};

	const VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
		.bindingCount = static_cast<u32>(BINDING_FLAGS.Size()),
		.pBindingFlags = BINDING_FLAGS.Data(),
	};

	const VkDescriptorSetLayoutCreateInfo layoutInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext = &bindingFlagsInfo,
		.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT,
		.bindingCount = static_cast<u32>(BINDINGS.Size()),
		.pBindings = BINDINGS.Data(),
	};

	VK_CHECK_THROW(
		vkCreateDescriptorSetLayout(device_.GetDevice(), &layoutInfo, nullptr, &descriptorSetLayout_),
		"failed to create descriptor set layout!"
	);
}

void UPipelineResourceManager::CreatePipelineLayout()
{
	const VkPushConstantRange pushConstantRange{
	   .stageFlags = VK_SHADER_STAGE_ALL,
	   .offset = 0,
	   .size = sizeof(UPushConstants),
	};

	const UArray setLayouts{ descriptorSetLayout_ };
	const UArray pushConstantRanges{ pushConstantRange };

	const VkPipelineLayoutCreateInfo pipelineLayoutInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,

		.setLayoutCount = static_cast<u32>(setLayouts.Size()),
		.pSetLayouts = setLayouts.Data(),

		.pushConstantRangeCount = static_cast<u32>(pushConstantRanges.Size()),
		.pPushConstantRanges = pushConstantRanges.Data(),
	};

	VK_CHECK_THROW(
		vkCreatePipelineLayout(device_.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout_),
		"failed to create pipeline layout!"
	);
}

void UPipelineResourceManager::CreateDescriptorPool()
{
	constexpr UArray POOL_SIZES{
		VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,	MAX_TEXTURES		    },
		VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,	MAX_TEXTURE_ARRAYS	    },
		VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLER,		MAX_SAMPLERS		    },
		VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLER,		MAX_SHADOW_SAMPLERS    },
		VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,	MAX_STORAGE_IMAGES	    },
	};

	const VkDescriptorPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT,
		.maxSets = 1,
		.poolSizeCount = static_cast<u32>(POOL_SIZES.Size()),
		.pPoolSizes = POOL_SIZES.Data(),
	};

	VK_CHECK_THROW(
		vkCreateDescriptorPool(device_.GetDevice(), &poolInfo, nullptr, &descriptorPool_),
		"failed to create descriptor pool!"
	);
}

void UPipelineResourceManager::CreateDescriptorSet()
{
	const VkDescriptorSetAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = descriptorPool_,
		.descriptorSetCount = 1,
		.pSetLayouts = &descriptorSetLayout_,
	};
	vkAllocateDescriptorSets(device_.GetDevice(), &allocInfo, &descriptorSet_);
}

u32 UPipelineResourceManager::AllocateSlot(ResourcePool& resourcePool)
{
	if (!resourcePool.freeSlots.IsEmpty())
	{
		u32 const slot{ resourcePool.freeSlots.Last() };
		resourcePool.freeSlots.Pop();
		return slot;
	}
	Check(Abort, resourcePool.nextSlot < resourcePool.maxSlots, "resourcePool has ran out of available slots!");
	return resourcePool.nextSlot++;
}

void UPipelineResourceManager::WriteDescriptorSet(
      VkSampler sampler
    , VkImageView imageView
	, VkImageLayout imageLayout
	, u32 binding
	, u32 slot
	, VkDescriptorType descriptorType) const
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
	vkUpdateDescriptorSets(device_.GetDevice(), 1, &write, 0, nullptr);
}
