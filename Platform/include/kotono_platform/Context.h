#pragma once
#include "AllocatedBuffer.h"
#include "AllocatedImage.h"
#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"
#include <kotono_common/Event.h>
#include <span>
#include <vector>
#include <vma/vk_mem_alloc.h>

class GContext final
{
public:
	void Init();
	void Cleanup();

	VkSampleCountFlagBits GetMSAASamples() const;
	VkPhysicalDevice& GetPhysicalDevice();
	VkDevice& GetDevice();
	VmaAllocator& GetAllocator();
	VkQueue& GetGraphicsQueue();
	VkQueue& GetPresentQueue();
	VkSurfaceKHR& GetSurface();

	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, u32 mipLevels);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, u32 width, u32 height);
	void GenerateMipmaps(VkImage image, VkFormat imageFormat, i32 texWidth, i32 texHeight, u32 mipLevels);
	KtQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
	KtSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;

	VkCommandBuffer BeginSingleTimeCommands() const;
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void ExecuteSingleTimeCommands();
	UEvent<>& GetEventExecuteSingleTimeCommands();
	void StagingUpload(void const* data, VkDeviceSize size, VkBuffer dstBuffer, VkDeviceSize dstOffset);
	
	VkFormat GetDepthFormat() const;

private:
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
	bool CheckDeviceFeatureSupport(VkPhysicalDevice device);
	void CreateLogicalDevice();
	
	void CreateSurface();
	void CreateAllocator();
	void CreateCommandPool();

	u32 FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const;
	bool HasStencilComponent(VkFormat format) const;
	VkSampleCountFlagBits GetMaxUsableSampleCount() const;
	bool GetIsComputerPluggedIn();

	VkFormat FindSupportedFormat(const std::span<const VkFormat> candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) const;
	VkFormat FindDepthFormat() const;

	void ClearDeletionQueue();

private:
	VkInstance instance_;
	VkDebugUtilsMessengerEXT debugMessenger_;
	VkSurfaceKHR surface_;

	VkPhysicalDevice physicalDevice_;
	VkDevice device_;

	VmaAllocator allocator_;

	VkQueue graphicsQueue_;
	VkQueue presentQueue_;

	VkSampleCountFlagBits msaaSamples_;

	VkCommandPool commandPool_;

	std::vector<VkCommandBuffer> singleTimeCommands_;
	UEvent<> eventExecuteSingleTimeCommands_;
	std::vector<UAllocatedBuffer> deletionQueue_;

	VkFormat depthFormat_;
};

inline GContext Context;
