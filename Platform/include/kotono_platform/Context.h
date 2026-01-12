#pragma once
#include "AllocatedBuffer.h"
#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <iostream>
#include <kotono_common/Event.h>
#include <vector>
#include <vma/vk_mem_alloc.h> 

class KtContext final
{
	friend class SCore;

private:
	void Init();
	void Cleanup();

public:
	VkSampleCountFlagBits GetMSAASamples() const;
	VkPhysicalDevice& GetPhysicalDevice();
	VkDevice& GetDevice();
	VmaAllocator& GetAllocator();
	VkQueue& GetGraphicsQueue();
	VkQueue& GetPresentQueue();
	VkSurfaceKHR& GetSurface();

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VmaAllocationCreateFlags flags, KtAllocatedBuffer& buffer, VmaMemoryUsage vmaUsage = VMA_MEMORY_USAGE_UNKNOWN) const;
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CreateImage(u32 width, u32 height, u32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VmaAllocation& imageAllocation) const;
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, u32 mipLevels);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, u32 width, u32 height);
	void GenerateMipmaps(VkImage image, VkFormat imageFormat, i32 texWidth, i32 texHeight, u32 mipLevels);
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, u32 mipLevels) const;
	KtQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
	KtSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;
	
	void ExecuteSingleTimeCommands();
	UEvent<>& GetEventExecuteSingleTimeCommands();

private:
	VkInstance instance_;
	VkDebugUtilsMessengerEXT debugMessenger_;
	VkSurfaceKHR surface_;

	VkPhysicalDevice physicalDevice_;
	VkDevice device_;

	VmaAllocator allocator_;

	VkQueue graphicsQueue_;
	VkQueue presentQueue_;

	VkCommandPool commandPool_;

	std::vector<VkCommandBuffer> singleTimeCommands_;
	UEvent<> eventExecuteSingleTimeCommands_;

	VkSampleCountFlagBits msaaSamples_;

	void CreateInstance();

	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const;
	void SetupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const;
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) const;
	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();
	void PickPhysicalDevice();
	bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	void CreateLogicalDevice();
	void CreateSurface();
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

	void CreateAllocator();

	void CreateCommandPool();
	VkCommandBuffer BeginSingleTimeCommands() const;
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

	u32 FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const;
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
	VkFormat FindDepthFormat() const;
	bool HasStencilComponent(VkFormat format) const;

	VkSampleCountFlagBits GetMaxUsableSampleCount() const;

	bool GetIsComputerPluggedIn();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};

inline KtContext Context;
