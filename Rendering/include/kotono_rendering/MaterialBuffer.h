#pragma once
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <vulkan/vulkan_core.h>
class GMaterialBuffer final
{
public:
	struct Material
	{
		u32 albedoIndex;
		u32 normalIndex;
		u32 ormIndex;
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
	UAllocatedBuffer dataBuffer_;
	u32 materialCount_;
};

inline GMaterialBuffer MaterialBuffer;
