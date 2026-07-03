#pragma once

#ifdef NDEBUG
#define VK_CHECK_THROW(x, msg) x
#else
#define VK_CHECK_THROW(x, msg) VulkanCheckAndThrow(x, msg)
#endif

enum VkResult;
void VulkanCheckAndThrow(const VkResult result, const char* message);