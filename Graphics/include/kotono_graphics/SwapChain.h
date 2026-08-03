#pragma once
#include <kotono_common/types.h>
#include <span>
#include <vector>
#include <vulkan/vulkan_core.h>
class GSwapChain final
{
public:
	struct Data
	{
		VkImage image;
		VkImageView imageView;
	};

public:
	void Init();
	void Cleanup();

	const VkSwapchainKHR& GetSwapChain() const;
	VkFormat GetFormat() const;
	VkExtent2D GetExtent() const;
	const Data& GetData(const u32 imageIndex) const;

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
	std::vector<Data> datas_;
};

inline GSwapChain SwapChain;
