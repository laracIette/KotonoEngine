#include "vk_utils.h"

#include <format>
#include <kotono_common/types.h>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

void VulkanCheckAndThrow(VkResult result, char const* message)
{
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error{ std::format("{0} (VkResult: {1})", message, static_cast<i32>(result)) };
	}
}

bool HasStencilComponent(VkFormat format) noexcept
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}
