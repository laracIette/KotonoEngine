#include "vk_utils.h"
#include <string>
#include <iostream>

void VulkanCheckAndCErr(const VkResult result, const char* message)
{
	if (result != VK_SUCCESS)
	{
		std::cerr << message << " (VkResult: " << result << ")" << std::endl;
	}
}

void VulkanCheckAndThrow(const VkResult result, const char* message)
{
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error(std::string(message) + " (VkResult: " + std::to_string(result) + ")");
	}
}
