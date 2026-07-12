#include "Sampler.h"
#include "PipelineResourceManager.h"
#include <kotono_io/Serializer.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>
#include <nlohmann/json.hpp>

USampler::USampler(const UPath& path)
	: path_{ path }
	, index_{}
{
	CreateSampler();

	nlohmann::json json{};
	SSerializer::Deserialize(json, path_);

	if (json["type"] == ESamplerType::Sampler)
	{
		index_ = PipelineResourceManager.RegisterSampler(sampler_);
	}
	else
	{
		index_ = PipelineResourceManager.RegisterShadowSampler(sampler_);
	}

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

	nlohmann::json json{};
	SSerializer::Deserialize(json, path_);

	const VkSamplerCreateInfo samplerInfo{
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter = json["magFilter"],
		.minFilter = json["minFilter"],
		.mipmapMode = json["mipmapMode"],
		.addressModeU = json["addressModeU"],
		.addressModeV = json["addressModeV"],
		.addressModeW = json["addressModeW"],
		.mipLodBias = json["mipLodBias"],
		.anisotropyEnable = json["anisotropyEnable"],
		.maxAnisotropy = properties.limits.maxSamplerAnisotropy,
		.compareEnable = json["compareEnable"],
		.compareOp = json["compareOp"],
		.minLod = json["minLod"],
		.maxLod = json["maxLod"],
		.borderColor = json["borderColor"],
		.unnormalizedCoordinates = json["unnormalizedCoordinates"],
	};
	VK_CHECK_THROW(
		vkCreateSampler(Context.GetDevice(), &samplerInfo, nullptr, &sampler_),
		"failed to create texture sampler!"
	);
}
