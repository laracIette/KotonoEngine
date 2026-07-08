#pragma once
#include "Lights.h"
#include <kotono_common/types.h>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
class GLightBuffer final
{
public:
	void Init();
	void Cleanup() const;

	u32 RegisterPointLight(const UPointLight& pointLight);
	void RegisterDirectionalLight(const UDirectionalLight& directionalLight);
	VkDeviceAddress GetAddress() const;

	u32 GetPointLightCount() const;
	const UDirectionalLight& GetDirectionalLight() const;

private:
	VkBuffer buffer_;
	VmaAllocation allocation_;
	VkDeviceAddress bda_;

	u32 pointLightCount_;
	UDirectionalLight directionalLight_;
};

inline GLightBuffer LightBuffer;
