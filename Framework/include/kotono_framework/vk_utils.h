#pragma once
#include <vulkan/vulkan_core.h>

#define VK_CHECK(x, msg) VulkanCheckAndCErr(x, msg)
#define VK_CHECK_THROW(x, msg) VulkanCheckAndThrow(x, msg)

void VulkanCheckAndCErr(const VkResult result, const char* message);
void VulkanCheckAndThrow(const VkResult result, const char* message);