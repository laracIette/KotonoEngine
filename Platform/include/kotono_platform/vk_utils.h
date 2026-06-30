#pragma once

#if defined (_DEBUG)
#define VK_CHECK_THROW(x, msg) VulkanCheckAndThrow(x, msg)
#else
#define VK_CHECK_THROW(x, msg) x
#endif

enum VkResult;
void VulkanCheckAndThrow(const VkResult result, const char* message);