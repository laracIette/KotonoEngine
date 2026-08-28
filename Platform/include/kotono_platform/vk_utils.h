#pragma once

#ifdef NDEBUG
#define VK_CHECK_THROW(x, msg) x
#else
#define VK_CHECK_THROW(x, msg) VulkanCheckAndThrow(x, msg)
#endif

enum VkResult;
void VulkanCheckAndThrow(VkResult result, char const* message);

enum VkFormat;
bool HasStencilComponent(VkFormat format) noexcept;