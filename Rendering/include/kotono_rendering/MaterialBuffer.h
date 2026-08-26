#pragma once
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <span>
#include <vulkan/vulkan_core.h>
struct UMaterialBufferData final
{
	u32 albedoIndex;
	u32 normalIndex;
	u32 ormIndex;
	u32 emissiveIndex;
	u32 materialType;
	u32 samplerIndex;
};
class UMaterialBuffer final
{
public:
	using Data = UMaterialBufferData;

public:
	void Init();
	void Cleanup() const;

	void UpdateBuffer(std::span<Data const> datas) const;

	VkDeviceAddress GetAddress() const;

private:
	UAllocatedBuffer dataBuffer_;
};
