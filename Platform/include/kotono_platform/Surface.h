#pragma once
#include <vulkan/vulkan_core.h>
class UContext;
class UWindow;
class USurface final
{
public:
	explicit USurface(UWindow& window, UContext& context);

	void Init();
	void Cleanup() const;

	VkExtent2D ChooseExtent(VkSurfaceCapabilitiesKHR const& capabilities) const;

	VkSurfaceKHR GetSurface() const { return surface_; }

private:
	void CreateSurface();

private:
	UWindow& window_;
	UContext& context_;
	VkSurfaceKHR surface_;
};