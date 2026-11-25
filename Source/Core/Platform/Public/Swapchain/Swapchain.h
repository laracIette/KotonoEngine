#pragma once

#include "AllocatedImage.h"
#include "Containers/List.h"
#include "types.h"

#include <vulkan/vulkan_core.h>

class UDevice;
class USurface;
class USwapchain final
{
public:
	explicit USwapchain(UDevice& device, USurface& surface);

	void Init();
	void Cleanup();

	auto GetAllocatedImage(u32 imageIndex) const -> UAllocatedImage const&;

	auto AcquireNextImage(u64 timeout, VkSemaphore semaphore, VkFence fence, u32& imageIndex) const -> VkResult;
	auto QueuePresent(VkSemaphore semaphore, u32 imageIndex) const -> VkResult;

	auto GetSwapchain() const -> VkSwapchainKHR { return swapchain_; }
	auto GetFormat() const -> VkFormat { return format_; }
	auto GetExtent() const -> VkExtent2D { return extent_; }

private:
	void CreateSwapchain();
	void CreateImageViews();

private:
	UDevice& device_;
	USurface& surface_;
	VkSwapchainKHR swapchain_;
	VkFormat format_;
	VkExtent2D extent_;
	UList<UAllocatedImage> allocatedImages_;
};
