#pragma once
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>
class USampler final
{
public:
	USampler(const UPath& path);
	~USampler();

	u32 GetIndex() const;

private:
	void CreateSampler();

private:
	const UPath path_;

	VkSampler sampler_;
	u32 index_;
};