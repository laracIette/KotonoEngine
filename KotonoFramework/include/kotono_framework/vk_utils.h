#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>

#define VK_CHECK(x, msg) VulkanCheckAndCErr(x, msg)
#define VK_CHECK_THROW(x, msg) VulkanCheckAndThrow(x, msg)

static void VulkanCheckAndCErr(const VkResult result, const char* message)
{
	if (result != VK_SUCCESS)
	{
		std::cerr << message << std::endl;
	}
}

static void VulkanCheckAndThrow(const VkResult result, const char* message)
{
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error(message);
	}
}