#pragma once
#include "AllocatedImage.h"
#include <kotono_common/types.h>
#include <span>
#include <vector>
#include <vulkan/vulkan_core.h>
class UDevice;
class USurface;
class USwapchain final
{
public:
	explicit USwapchain(UDevice& device, USurface& surface);

	void Init();
	void Cleanup();

	UAllocatedImage const& GetAllocatedImage(u32 imageIndex) const;

	VkResult AcquireNextImage(u64 timeout, VkSemaphore semaphore, VkFence fence, u32& imageIndex) const;
	VkResult QueuePresent(VkSemaphore semaphore, u32 imageIndex) const;

	VkSwapchainKHR	GetSwapchain() const { return swapchain_; }
	VkFormat		GetFormat() const { return format_; }
	VkExtent2D		GetExtent() const { return extent_; }

private:
	void CreateSwapchain();
	void CreateImageViews();

private:
	UDevice& device_;
	USurface& surface_;
	VkSwapchainKHR swapchain_;
	VkFormat format_;
	VkExtent2D extent_;
	std::vector<UAllocatedImage> allocatedImages_;
};
