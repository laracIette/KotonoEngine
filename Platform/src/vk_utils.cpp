#include "vk_utils.h"
#include <print>
#include <kotono_common/types.h>

void VulkanCheckAndCErr(const VkResult result, const char* message)
{
	if (result != VK_SUCCESS)
	{
		std::println("{0} (VkResult: {1})", message, static_cast<i32>(result));
	}
}

void VulkanCheckAndThrow(const VkResult result, const char* message)
{
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error{ std::format("{0} (VkResult: {1})", message, static_cast<i32>(result)) };
	}
}
