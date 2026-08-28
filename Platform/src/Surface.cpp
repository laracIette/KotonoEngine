#include "Surface.h"

#include "Context.h"
#include "vk_utils.h"
#include "Window.h"
#include <algorithm>
#include <GLFW/glfw3.h>
#include <kotono_common/types.h>
#include <limits>

USurface::USurface(UWindow& window, UContext& context)
	: window_{ window }
	, context_{ context }
{
}

void USurface::Init()
{
	CreateSurface();
}

void USurface::Cleanup() const
{
	vkDestroySurfaceKHR(context_.GetInstance(), surface_, nullptr);
}

VkExtent2D USurface::ChooseExtent(VkSurfaceCapabilitiesKHR const& capabilities) const
{
	if (capabilities.currentExtent.width != std::numeric_limits<u32>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		i32 width, height;
		glfwGetFramebufferSize(window_.GetGLFWWindow(), &width, &height);

		VkExtent2D actualExtent
		{
			static_cast<u32>(width),
			static_cast<u32>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

void USurface::CreateSurface()
{
	VK_CHECK_THROW(
		glfwCreateWindowSurface(context_.GetInstance(), window_.GetGLFWWindow(), nullptr, &surface_),
		"failed to create window surface!"
	);
}
