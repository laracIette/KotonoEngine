#include "SwapChain.h"
#include <array>
#include <glm/glm.hpp>
#include <kotono_common/log.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/vk_utils.h>
#include <kotono_platform/Window.h>

void GSwapChain::Init()
{
	Create();
	CreateImageViews();
}

void GSwapChain::Cleanup()
{
	for (const auto& allocatedImage : allocatedImages_)
	{
		vkDestroyImageView(Context.GetDevice(), allocatedImage.imageView, nullptr);
	}
	allocatedImages_.clear();

	vkDestroySwapchainKHR(Context.GetDevice(), swapChain_, nullptr);
	swapChain_ = VK_NULL_HANDLE;
}

const VkSwapchainKHR& GSwapChain::GetSwapChain() const
{
	return swapChain_;
}

VkFormat GSwapChain::GetFormat() const
{
	return format_;
}

VkExtent2D GSwapChain::GetExtent() const
{
	return extent_;
}

const UAllocatedImage& GSwapChain::GetAllocatedImage(const u32 imageIndex) const
{
	return allocatedImages_[imageIndex];
}

void GSwapChain::Create()
{
	const auto swapChainSupport{ Context.QuerySwapChainSupport(Context.GetPhysicalDevice()) };

	const auto surfaceFormat{ ChooseSurfaceFormat(swapChainSupport.formats) };
	const auto presentMode{ ChoosePresentMode(swapChainSupport.presentModes) };
	const auto extent{ ChooseExtent(swapChainSupport.capabilities) };

	u32 imageCount{ swapChainSupport.capabilities.minImageCount + 1 };
	if (swapChainSupport.capabilities.maxImageCount > 0
		&& imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	KT_LOG(ELogImportanceLevel::High, "Graphics", "swap chain image count: {0}", imageCount);

	const KtQueueFamilyIndices indices{ Context.FindQueueFamilies(Context.GetPhysicalDevice()) };
	const std::array queueFamilyIndices{ indices.graphicsFamily.value(), indices.presentFamily.value() };

	VkSharingMode imageSharingMode;
	u32 queueFamilyIndexCount;
	const u32* pQueueFamilyIndices;
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

	const VkSwapchainCreateInfoKHR createInfo{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = Context.GetSurface(),

		.minImageCount = imageCount,
		.imageFormat = surfaceFormat.format,
		.imageColorSpace = surfaceFormat.colorSpace,

		.imageExtent = extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

		.imageSharingMode = imageSharingMode,
		.queueFamilyIndexCount = queueFamilyIndexCount,
		.pQueueFamilyIndices = pQueueFamilyIndices,

		.preTransform = swapChainSupport.capabilities.currentTransform,

		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, // Used for blending window (here opaque)
		.presentMode = presentMode,
		.clipped = VK_TRUE, // Discard pixels that are hidden by other windows
		.oldSwapchain = VK_NULL_HANDLE,
	};
	VK_CHECK_THROW(
		vkCreateSwapchainKHR(Context.GetDevice(), &createInfo, nullptr, &swapChain_),
		"failed to create swap chain!"
	);

	vkGetSwapchainImagesKHR(Context.GetDevice(), swapChain_, &imageCount, nullptr);
	std::vector<VkImage> swapChainImages{ imageCount };
	vkGetSwapchainImagesKHR(Context.GetDevice(), swapChain_, &imageCount, swapChainImages.data());

	allocatedImages_.resize(imageCount);
	for (size i{ 0 }; i < imageCount; ++i)
	{
		allocatedImages_[i].image = swapChainImages[i];
	}

	format_ = surfaceFormat.format;
	extent_ = extent;
}

VkSurfaceFormatKHR GSwapChain::ChooseSurfaceFormat(const std::span<const VkSurfaceFormatKHR> availableFormats) const
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

VkPresentModeKHR GSwapChain::ChoosePresentMode(const std::span<const VkPresentModeKHR> availablePresentModes) const
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

VkExtent2D GSwapChain::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
{
	if (capabilities.currentExtent.width != std::numeric_limits<u32>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(Window.GetGLFWWindow(), &width, &height);

		VkExtent2D actualExtent
		{
			static_cast<u32>(width),
			static_cast<u32>(height)
		};

		actualExtent.width = glm::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = glm::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

void GSwapChain::CreateImageViews()
{
	for (auto& allocatedImage : allocatedImages_)
	{
		Context.CreateImageView(allocatedImage, {
			.arrayLayers = 1,
			.mipLevels = 1,
			.format = format_,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.aspect = VK_IMAGE_ASPECT_COLOR_BIT,
		});
	}
}
