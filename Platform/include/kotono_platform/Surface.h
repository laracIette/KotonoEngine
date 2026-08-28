#pragma once
#include <vulkan/vulkan_core.h>
class UWindow;
class USurface final
{
public:
	explicit USurface(UWindow& window);

	void Init(VkInstance instance);
	void Cleanup(VkInstance instance) const;

	VkExtent2D ChooseExtent(VkSurfaceCapabilitiesKHR const& capabilities) const;

	VkSurfaceKHR GetSurface() const { return surface_; }

private:
	void CreateSurface(VkInstance instance);

private:
	UWindow& window_;
	VkSurfaceKHR surface_;
};