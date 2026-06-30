#include "vk_utils.h"
#include <stdexcept>
#include <format>
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>

void VulkanCheckAndThrow(const VkResult result, const char* message)
{
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error{ std::format("{0} (VkResult: {1})", message, static_cast<i32>(result)) };
	}
}
