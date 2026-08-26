#include "Sampler.h"

#include <kotono_io/serialize_base.h>
#include <kotono_io/Serializer.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>
#include <nlohmann/json.hpp>

ASampler::ASampler(UPath const& path)
	: AAsset(path)
	, index_{}
{
	CreateSampler();
}

ASampler::~ASampler()
{
	vkDestroySampler(Context.GetDevice(), sampler_, nullptr);
}

u32 ASampler::GetIndex() const
{
	return index_;
}

void ASampler::SetIndex(u32 index)
{
	index_ = index;
}

VkSampler ASampler::GetSampler() const
{
	return sampler_;
}

ASampler::EType ASampler::GetType() const
{
	nlohmann::json json{};
	SSerializer::Deserialize(json, GetPath());

	return UDeserialize<EType>{}(json["type"]);
}

void ASampler::CreateSampler()
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(Context.GetPhysicalDevice(), &properties);

	nlohmann::json json{};
	SSerializer::Deserialize(json, GetPath());

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
