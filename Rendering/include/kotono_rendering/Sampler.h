#pragma once
#include <kotono_common/Asset.h>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>
enum class ESamplerType : u8
{
	Sampler,
	ShadowSampler,
};
class ASampler final : public AAsset
{
public:
	ASampler(UPath const& path);
	~ASampler() override;

	u32 GetIndex() const;

private:
	void CreateSampler();

private:
	VkSampler sampler_;
	u32 index_;
};