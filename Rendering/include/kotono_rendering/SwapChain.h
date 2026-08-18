#pragma once
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedImage.h>
#include <span>
#include <vector>
#include <vulkan/vulkan_core.h>
class USwapChain final
{
public:
	void Init();
	void Cleanup();

	const VkSwapchainKHR& GetSwapChain() const;
	VkFormat GetFormat() const;
	VkExtent2D GetExtent() const;
	const UAllocatedImage& GetAllocatedImage(const u32 imageIndex) const;

private:
	void Create();
	VkSurfaceFormatKHR ChooseSurfaceFormat(const std::span<const VkSurfaceFormatKHR> availableFormats) const;
	VkPresentModeKHR ChoosePresentMode(const std::span<const VkPresentModeKHR> availablePresentModes) const;
	VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	void CreateImageViews();

private:
	VkSwapchainKHR swapChain_;
	VkFormat format_;
	VkExtent2D extent_;
	std::vector<UAllocatedImage> allocatedImages_;
};
