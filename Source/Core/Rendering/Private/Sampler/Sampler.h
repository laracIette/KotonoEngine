#pragma once
#include <Asset/Asset.h>

#include <types.h>
#include <vulkan/vulkan_core.h>

class UDevice;

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

	void Init(UDevice& device);
	void Cleanup(UDevice& device) const;

	u32 GetIndex() const;
	void SetIndex(u32 index);

	VkSampler GetSampler() const;
	EType GetType() const;

private:
	void CreateSampler(UDevice& device);

private:
	VkSampler sampler_;
	u32 index_;
};