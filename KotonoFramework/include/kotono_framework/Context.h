#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#define VK_USE_PLATFORM_WIN32_KHR
#include <vma/vk_mem_alloc.h> 
#include <vector>
#include <iostream>
#include <cstdint>
#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"
#include "AllocatedBuffer.h"
#include "Event.h"

class KtContext final
{
public:
	void Init();
	void Cleanup();

	const VkSampleCountFlagBits GetMSAASamples() const;
	VkPhysicalDevice& GetPhysicalDevice();
	VkDevice& GetDevice();
	VmaAllocator& GetAllocator();
	VkQueue& GetGraphicsQueue();
	VkQueue& GetPresentQueue();
	VkSurfaceKHR& GetSurface();

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VmaAllocationCreateFlags flags, KtAllocatedBuffer& buffer, VmaMemoryUsage vmaUsage = VMA_MEMORY_USAGE_UNKNOWN) const;
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VmaAllocation& imageAllocation) const;
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	const VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) const;
	const KtQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
	const KtSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;
	
	void ExecuteSingleTimeCommands();
	KtEvent<>& GetEventExecuteSingleTimeCommands();

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
	KtEvent<> eventExecuteSingleTimeCommands_;

	VkSampleCountFlagBits msaaSamples_;

	void CreateInstance();

	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const;
	void SetupDebugMessenger();
	const VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const;
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) const;
	const bool CheckValidationLayerSupport();
	const std::vector<const char*> GetRequiredExtensions();
	void PickPhysicalDevice();
	const bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	void CreateLogicalDevice();
	void CreateSurface();
	const VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
	const VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
	const VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

	void CreateAllocator();

	void CreateCommandPool();
	VkCommandBuffer BeginSingleTimeCommands() const;
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

	const uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	const VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
	const VkFormat FindDepthFormat() const;
	const bool HasStencilComponent(VkFormat format) const;

	const VkSampleCountFlagBits GetMaxUsableSampleCount() const;

	const bool GetIsComputerPluggedIn();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};
