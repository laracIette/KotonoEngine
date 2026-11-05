#pragma once

#include <vulkan/vulkan_core.h>

#ifdef NDEBUG
#define VK_CHECK_THROW(x, msg) x
#else
#define VK_CHECK_THROW(x, msg) VulkanCheckAndThrow(x, msg)
#endif

void VulkanCheckAndThrow(VkResult result, char const* message);
bool HasStencilComponent(VkFormat format) noexcept;