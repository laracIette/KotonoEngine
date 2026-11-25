#pragma once

#include "AllocatedBuffer.h"
#include "Containers/List.h"
#include "QueueFamilyIndices.h"
#include "SwapchainSupportDetails.h"
#include "types.h"

#include <unordered_set>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

struct UAllocatedImage;
struct UAllocatedImageCreateInfo;
class UContext;

class UDevice final
{
public:
	explicit UDevice(UContext& context);

	void Init(VkSurfaceKHR mainSurface);
	void Cleanup() const;

	void ExecuteSingleTimeCommands();

	void CopyBuffer(UAllocatedBuffer const& srcBuffer, UAllocatedBuffer const& dstBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size) const;
	void CopyBufferToImage(UAllocatedBuffer const& srcBuffer, UAllocatedImage const& dstImage, u32 width, u32 height) const;

	void GenerateMipmaps(UAllocatedImage const& image, VkFormat format, i32 width, i32 height, u32 mipLevels) const;
	void TransitionImageLayout(UAllocatedImage const& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, u32 mipLevels) const;

	void StageBufferForDeletion(UAllocatedBuffer const& allocatedBuffer);

	void StagingUpload(void const* data, VkDeviceSize size, UAllocatedBuffer const& dstBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset);

	auto CreateAllocatedBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateFlags allocFlags, VmaMemoryUsage memUsage = VMA_MEMORY_USAGE_AUTO) const -> UAllocatedBuffer;
	auto CreateAllocatedImage(UAllocatedImageCreateInfo const& createInfo) const -> UAllocatedImage;
	void CreateImage(UAllocatedImage& allocatedImage, UAllocatedImageCreateInfo const& createInfo) const;
	void CreateImageView(UAllocatedImage& allocatedImage, UAllocatedImageCreateInfo const& createInfo) const;

	void CleanupAllocatedBuffer(UAllocatedBuffer const& allocatedBuffer) const;
	void CleanupAllocatedImage(UAllocatedImage const& allocatedImage) const;
	void CleanupImage(UAllocatedImage const& allocatedImage) const;
	void CleanupImageView(UAllocatedImage const& allocatedImage) const;

	auto QuerySwapchainSupportDetails(VkSurfaceKHR surface) const -> USwapchainSupportDetails;

	auto GetPhysicalDevice() const -> VkPhysicalDevice { return physicalDevice_; }
	auto GetDevice() const -> VkDevice { return device_; }
	auto GetAllocator() const -> VmaAllocator { return allocator_; }
	auto GetCommandPool() const -> VkCommandPool { return commandPool_; }
	auto GetGraphicsQueue() const -> VkQueue { return graphicsQueue_; }
	auto GetPresentQueue() const -> VkQueue { return presentQueue_; }

	auto GetMaxSamplerAnisotropy() const -> f32 { return maxSamplerAnisotropy_; }
	auto GetDepthFormat() const -> VkFormat { return depthFormat_; }
	auto GetMSAASamples() const -> VkSampleCountFlagBits { return msaaSamples_; }

	auto GetQueueFamilyIndices() const -> UQueueFamilyIndices const& { return queueFamilyIndices_; }

private:
	void CreatePhysicalDevice(VkSurfaceKHR mainSurface);
	void CreateDevice();
	void CreateAllocator();
	void CreateCommandPool();

	void CreateSingleTimeCommandBuffer();
	void BeginSingleTimeCommandBuffer() const;
	void EndSingleTimeCommandBuffer() const;
	void ClearDeletionQueue();

	auto FormatSupported(VkFormat format, VkFormatFeatureFlags features) const -> b8;
	void PopulateSupportedMipmapFormats();
	auto FindSupportedDepthFormat() const -> VkFormat;
	auto FindMaxSamplerAnisotropy() const -> f32;
	auto GetMaxUsableSampleCount() const -> VkSampleCountFlagBits;

private:
	UContext& context_;

	VkPhysicalDevice physicalDevice_;
	VkDevice device_;
	VmaAllocator allocator_;
	VkCommandPool commandPool_;

	VkFormat depthFormat_;
	VkSampleCountFlagBits msaaSamples_;

	UQueueFamilyIndices queueFamilyIndices_;

	VkQueue graphicsQueue_;
	VkQueue presentQueue_;

	VkCommandBuffer singleTimeCommandBuffer_;
	UList<UAllocatedBuffer> deletionQueue_;

	std::unordered_set<VkFormat> supportedMipmapFormats_;
	f32 maxSamplerAnisotropy_;
};