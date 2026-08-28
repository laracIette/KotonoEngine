#pragma once
#include "QueueFamilyIndices.h"
#include "SwapchainSupportDetails.h"
#include <kotono_common/types.h>
#include <unordered_set>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
struct UAllocatedBuffer;
struct UAllocatedImage;
struct UAllocatedImageCreateInfo;
class USurface;
class UDevice final
{
public:
	explicit UDevice(USurface& surface);

	void Init(VkInstance instance);
	void Cleanup() const;

	void ExecuteSingleTimeCommands();

	void CopyBuffer(UAllocatedBuffer const& srcBuffer, UAllocatedBuffer const& dstBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size) const;
	void CopyBufferToImage(UAllocatedBuffer const& srcBuffer, UAllocatedImage const& dstImage, u32 width, u32 height) const;

	void GenerateMipmaps(UAllocatedImage const& image, VkFormat format, i32 width, i32 height, u32 mipLevels) const;
	void TransitionImageLayout(UAllocatedImage const& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, u32 mipLevels) const;

	void StageBufferForDeletion(UAllocatedBuffer const& allocatedBuffer);

	void StagingUpload(void const* data, VkDeviceSize size, UAllocatedBuffer const& dstBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset);

	UAllocatedBuffer CreateAllocatedBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateFlags allocFlags, VmaMemoryUsage memUsage = VMA_MEMORY_USAGE_AUTO) const;
	UAllocatedImage CreateAllocatedImage(UAllocatedImageCreateInfo const& createInfo) const;
	void CreateImage(UAllocatedImage& allocatedImage, UAllocatedImageCreateInfo const& createInfo) const;
	void CreateImageView(UAllocatedImage& allocatedImage, UAllocatedImageCreateInfo const& createInfo) const;

	void CleanupAllocatedBuffer(UAllocatedBuffer const& allocatedBuffer) const;
	void CleanupAllocatedImage(UAllocatedImage const& allocatedImage) const;
	void CleanupImage(UAllocatedImage const& allocatedImage) const;
	void CleanupImageView(UAllocatedImage const& allocatedImage) const;

	// Getters
	VkPhysicalDevice	GetPhysicalDevice() const { return physicalDevice_; }
	VkDevice			GetDevice() const { return device_; }
	VmaAllocator		GetAllocator() const { return allocator_; }
	VkCommandPool		GetCommandPool() const { return commandPool_; }
	VkQueue				GetGraphicsQueue() const { return graphicsQueue_; }
	VkQueue				GetPresentQueue() const { return presentQueue_; }

	f32						GetMaxSamplerAnisotropy() const { return maxSamplerAnisotropy_; }
	VkFormat				GetDepthFormat() const { return depthFormat_; }
	VkSampleCountFlagBits	GetMSAASamples() const { return msaaSamples_; }

	UQueueFamilyIndices	const&		GetQueueFamilyIndices() const { return queueFamilyIndices_; }
	USwapchainSupportDetails const&	GetSwapchainSupportDetails() const { return swapchainSupportDetails_; }

private:
	void CreatePhysicalDevice(VkInstance instance);
	void CreateDevice();
	void CreateAllocator(VkInstance instance);
	void CreateCommandPool();

	void CreateSingleTimeCommandBuffer();
	void BeginSingleTimeCommandBuffer() const;
	void EndSingleTimeCommandBuffer() const;
	void ClearDeletionQueue();

	b8 FormatSupported(VkFormat format, VkFormatFeatureFlags features) const;
	void PopulateSupportedMipmapFormats();
	VkFormat FindSupportedDepthFormat() const;
	f32 FindMaxSamplerAnisotropy() const;
	VkSampleCountFlagBits GetMaxUsableSampleCount() const;

private:
	USurface& surface_;

	VkPhysicalDevice physicalDevice_;
	VkDevice device_;
	VmaAllocator allocator_;
	VkCommandPool commandPool_;

	VkFormat depthFormat_;
	VkSampleCountFlagBits msaaSamples_;

	UQueueFamilyIndices queueFamilyIndices_;
	USwapchainSupportDetails swapchainSupportDetails_;

	VkQueue graphicsQueue_;
	VkQueue presentQueue_;

	VkCommandBuffer singleTimeCommandBuffer_;
	std::vector<UAllocatedBuffer> deletionQueue_;

	std::unordered_set<VkFormat> supportedMipmapFormats_;
	f32 maxSamplerAnisotropy_;
};