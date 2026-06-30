#include "Sampler.h"
#include "PipelineResourceManager.h"
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>

USampler::USampler(const UPath& path)
	: path_{ path }
	, index_{}
{
	CreateSampler();

	index_ = PipelineResourceManager.RegisterSampler(sampler_);
}

USampler::~USampler()
{
	vkDestroySampler(Context.GetDevice(), sampler_, nullptr);
}

u32 USampler::GetIndex() const
{
	return index_;
}

void USampler::CreateSampler()
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(Context.GetPhysicalDevice(), &properties);

	const VkSamplerCreateInfo samplerInfo{
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter = VK_FILTER_LINEAR,
		.minFilter = VK_FILTER_LINEAR,
		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.mipLodBias = 0.0f, // Optional
		.anisotropyEnable = VK_TRUE,
		.maxAnisotropy = properties.limits.maxSamplerAnisotropy,
		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_ALWAYS,
		.minLod = 0.0f,
		.maxLod = VK_LOD_CLAMP_NONE,
		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
	};
	VK_CHECK_THROW(
		vkCreateSampler(Context.GetDevice(), &samplerInfo, nullptr, &sampler_),
		"failed to create texture sampler!"
	);
}
