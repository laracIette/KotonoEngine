#include "Swapchain.h"

#include "Device.h"
#include "Surface.h"
#include "vk_utils.h"
#include <array>
#include <kotono_common/log.h>

static VkSurfaceFormatKHR chooseSurfaceFormat(std::span<VkSurfaceFormatKHR const> availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

static VkPresentModeKHR choosePresentMode(std::span<VkPresentModeKHR const> availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

USwapchain::USwapchain(UDevice& device, USurface& surface)
	: device_{ device }
	, surface_{ surface }
{
}

void USwapchain::Init()
{
	CreateSwapchain();
	CreateImageViews();
}

void USwapchain::Cleanup()
{
	for (auto const& allocatedImage : allocatedImages_)
	{
		device_.CleanupImageView(allocatedImage);
	}
	allocatedImages_.clear();

	vkDestroySwapchainKHR(device_.GetDevice(), swapchain_, nullptr);
}

UAllocatedImage const& USwapchain::GetAllocatedImage(u32 imageIndex) const
{
	return allocatedImages_[imageIndex];
}

VkResult USwapchain::AcquireNextImage(u64 timeout, VkSemaphore semaphore, VkFence fence, u32& imageIndex) const
{
	// Index of the physical device in the logical device
	constexpr u32 DEVICE_INDEX{ 0 };

	VkAcquireNextImageInfoKHR const acquireInfo{
		.sType = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR,
		.swapchain = swapchain_,
		.timeout = timeout,
		.semaphore = semaphore,
		.fence = fence,
		.deviceMask = 1u << DEVICE_INDEX,
	};
	return vkAcquireNextImage2KHR(device_.GetDevice(), &acquireInfo, &imageIndex);
}

VkResult USwapchain::QueuePresent(VkSemaphore semaphore, u32 imageIndex) const
{
	VkPresentInfoKHR const presentInfo{
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,

		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &semaphore,

		.swapchainCount = 1,
		.pSwapchains = &swapchain_,
		.pImageIndices = &imageIndex,
	};
	return vkQueuePresentKHR(device_.GetPresentQueue(), &presentInfo);
}

void USwapchain::CreateSwapchain()
{
	auto const swapchainSupport{ device_.GetSwapchainSupportDetails() };

	auto const surfaceFormat{ chooseSurfaceFormat(swapchainSupport.formats) };
	auto const presentMode{ choosePresentMode(swapchainSupport.presentModes) };
	auto const extent{ surface_.ChooseExtent(swapchainSupport.capabilities) };

	u32 imageCount{ swapchainSupport.capabilities.minImageCount + 1 };
	if (swapchainSupport.capabilities.maxImageCount > 0
		&& imageCount > swapchainSupport.capabilities.maxImageCount)
	{
		imageCount = swapchainSupport.capabilities.maxImageCount;
	}

	KT_LOG(ELogImportanceLevel::High, "Graphics", "swap chain image count: {0}", imageCount);

	UQueueFamilyIndices const indices{ device_.GetQueueFamilyIndices() };
	std::array const queueFamilyIndices{ indices.graphicsFamily.value(), indices.presentFamily.value() };

	VkSharingMode imageSharingMode;
	u32 queueFamilyIndexCount;
	u32 const* pQueueFamilyIndices;
	if (indices.graphicsFamily != indices.presentFamily)
	{
		imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		queueFamilyIndexCount = static_cast<u32>(queueFamilyIndices.size());
		pQueueFamilyIndices = queueFamilyIndices.data();
	}
	else
	{
		imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		queueFamilyIndexCount = 0;
		pQueueFamilyIndices = VK_NULL_HANDLE;
	}

	VkSwapchainCreateInfoKHR const createInfo{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface_.GetSurface(),

		.minImageCount = imageCount,
		.imageFormat = surfaceFormat.format,
		.imageColorSpace = surfaceFormat.colorSpace,

		.imageExtent = extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

		.imageSharingMode = imageSharingMode,
		.queueFamilyIndexCount = queueFamilyIndexCount,
		.pQueueFamilyIndices = pQueueFamilyIndices,

		.preTransform = swapchainSupport.capabilities.currentTransform,

		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, // Used for blending window (here opaque)
		.presentMode = presentMode,
		.clipped = VK_TRUE, // Discard pixels that are hidden by other windows
		.oldSwapchain = VK_NULL_HANDLE,
	};

	VK_CHECK_THROW(
		vkCreateSwapchainKHR(device_.GetDevice(), &createInfo, nullptr, &swapchain_),
		"failed to create swap chain!"
	);

	vkGetSwapchainImagesKHR(device_.GetDevice(), swapchain_, &imageCount, nullptr);
	std::vector<VkImage> swapchainImages{ imageCount };
	vkGetSwapchainImagesKHR(device_.GetDevice(), swapchain_, &imageCount, swapchainImages.data());

	allocatedImages_.resize(imageCount);
	for (size i{ 0 }; i < imageCount; ++i)
	{
		allocatedImages_[i].image = swapchainImages[i];
	}

	format_ = surfaceFormat.format;
	extent_ = extent;
}

void USwapchain::CreateImageViews()
{
	for (auto& allocatedImage : allocatedImages_)
	{
		device_.CreateImageView(allocatedImage, {
			.arrayLayers = 1,
			.mipLevels = 1,
			.format = format_,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.aspect = VK_IMAGE_ASPECT_COLOR_BIT,
		});
	}
}
