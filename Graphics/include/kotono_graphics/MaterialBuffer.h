#pragma once
#include <kotono_common/types.h>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class GMaterialBuffer final
{
public:
	struct Material
	{
		u32 albedoIndex;
		u32 normalIndex;
		u32 roughnessIndex;
		u32 emissiveIndex;
		u32 materialType;
		u32 samplerIndex;
	};

public:
	void Init();
	void Cleanup() const;

	u32 RegisterMaterial(const Material& material);
	VkDeviceAddress GetAddress() const;

private:
	VkBuffer buffer_;
	VmaAllocation allocation_;
	VkDeviceAddress bda_;

	u32 materialCount_;
};

inline GMaterialBuffer MaterialBuffer;
