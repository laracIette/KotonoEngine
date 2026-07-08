#pragma once
#include "Lights.h"
#include <kotono_common/types.h>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class GLightBuffers final
{
public:
	struct LightBuffer
	{
		VkBuffer buffer;
		VmaAllocation allocation;
		VkDeviceAddress bda;
		u32 count;
	};

public:
	void Init();
	void Cleanup() const;

	u32 RegisterDirectionalLight(const UDirectionalLight& directionalLight);
	u32 RegisterPointLight(const UPointLight& pointLight);

	VkDeviceAddress GetDirectionalLightAddress() const;
	VkDeviceAddress GetPointLightAddress() const;

	u32 GetDirectionalLightCount() const;
	u32 GetPointLightCount() const;

private:
	void CreateBuffer(LightBuffer& lightBuffer, const VkDeviceSize size);
	u32 Register(LightBuffer& lightBuffer, const void* data, const VkDeviceSize size, const u32 maxCount);

private:
	LightBuffer directionalLightBuffer_;
	LightBuffer pointLightBuffer_;
};

inline GLightBuffers LightBuffers;
