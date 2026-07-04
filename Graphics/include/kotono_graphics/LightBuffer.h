#pragma once
#include <glm/vec3.hpp>
#include <kotono_common/types.h>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
enum class ELightType : u8
{
	Directional,
	Point,
	Spot,
};
class GLightBuffer final
{
public:
	struct Light
	{
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 color;
		f32 intensity;
		f32 range;
		f32 innerCone, outerCone;
		u32 type;
	};

public:
	void Init();
	void Cleanup() const;

	u32 RegisterLight(const Light& light);
	VkDeviceAddress GetAddress() const;

	u32 GetLightCount() const;

private:
	VkBuffer buffer_;
	VmaAllocation allocation_;
	VkDeviceAddress bda_;

	u32 lightCount_;
};

inline GLightBuffer LightBuffer;
