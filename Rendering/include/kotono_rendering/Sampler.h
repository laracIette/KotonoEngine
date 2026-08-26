#pragma once
#include <kotono_common/Asset.h>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>
class ASampler final : public AAsset
{
public:
	enum class EType : u8
	{
		Sampler,
		ShadowSampler,
	};

public:
	ASampler(UPath const& path);

	void Init(VkDevice device, f32 maxAnisotropy);
	void Cleanup(VkDevice device) const;

	u32 GetIndex() const;
	void SetIndex(u32 index);

	VkSampler GetSampler() const;
	EType GetType() const;

private:
	void CreateSampler(VkDevice device, f32 maxAnisotropy);

private:
	VkSampler sampler_;
	u32 index_;
};