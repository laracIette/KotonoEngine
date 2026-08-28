#include "Context.h"

#include "vk_utils.h"
#include <array>
#include <GLFW/glfw3.h>
#include <kotono_common/log.h>
#include <kotono_common/types.h>
#include <print>
#include <vector>

static constexpr std::array VALIDATION_LAYERS
{
	"VK_LAYER_KHRONOS_validation",
};

static constexpr std::array VALIDATION_FEATURES
{
	VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT,
};

#ifdef NDEBUG
static constexpr b8 ENABLE_VALIDATION_LAYERS{ false };
#else
static constexpr b8 ENABLE_VALIDATION_LAYERS{ true };
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
	, VkDebugUtilsMessageTypeFlagsEXT messageType
	, VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData
	, void* pUserData
)
{
	std::println("[Vulkan Validation Layer] {0}", pCallbackData->pMessage);
	return VK_FALSE;
}

static VkDebugUtilsMessengerCreateInfoEXT getDebugMessengerCreateInfo()
{
	return{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.pNext = VK_NULL_HANDLE,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = debugCallback,
	};
}

static VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	const auto func{ reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
		vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT")
	) };

	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	const auto func{ reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
		vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT") 
	) };

	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

static b8 checkValidationLayerSupport()
{
	u32 layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers{ layerCount };
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : VALIDATION_LAYERS)
	{
		bool layerFound{ false };

		for (const auto& layerProperties : availableLayers)
		{
			if (std::strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

static std::vector<const char*> getRequiredExtensions()
{
	u32 glfwExtensionCount{ 0 };
	const char** glfwExtensions{ glfwGetRequiredInstanceExtensions(&glfwExtensionCount) };

	std::vector<const char*> extensions{ glfwExtensions, glfwExtensions + glfwExtensionCount };

	if constexpr (ENABLE_VALIDATION_LAYERS)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void UContext::Init()
{
	CreateInstance();
	SetupDebugMessenger();
}

void UContext::Cleanup() const
{
	if constexpr (ENABLE_VALIDATION_LAYERS)
	{
		destroyDebugUtilsMessengerEXT(instance_, debugMessenger_, nullptr);
	}

	vkDestroyInstance(instance_, nullptr);

	KT_LOG(ELogImportanceLevel::High, "Platform", "cleaned up context");
}

void UContext::CreateInstance()
{
	if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport())
	{
		throw std::runtime_error{ "validation layers requested, but not available!" };
	}

	VkApplicationInfo const appInfo{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Kotono Engine Application",
		.applicationVersion = VK_MAKE_VERSION(0, 0, 1),
		.pEngineName = "Kotono Engine",
		.engineVersion = VK_MAKE_VERSION(0, 0, 1),
		.apiVersion = VK_API_VERSION_1_4,
	};

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	VkValidationFeaturesEXT validationFeatures;
	const void* pNext;
	u32 enabledLayerCount;

	if constexpr (ENABLE_VALIDATION_LAYERS)
	{
		debugCreateInfo = getDebugMessengerCreateInfo();

		validationFeatures = {
			.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
			.pNext = &debugCreateInfo,
			.enabledValidationFeatureCount = static_cast<u32>(VALIDATION_FEATURES.size()),
			.pEnabledValidationFeatures = VALIDATION_FEATURES.data(),
		};

		pNext = &validationFeatures;
		enabledLayerCount = static_cast<u32>(VALIDATION_LAYERS.size());
	}
	else
	{
		pNext = VK_NULL_HANDLE;
		enabledLayerCount = 0;
	}

	const auto extensions{ getRequiredExtensions() };

	const VkInstanceCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = pNext,
		.pApplicationInfo = &appInfo,
		
		.enabledLayerCount = enabledLayerCount,
		.ppEnabledLayerNames = VALIDATION_LAYERS.data(),
		
		.enabledExtensionCount = static_cast<u32>(extensions.size()),
		.ppEnabledExtensionNames = extensions.data(),
	};

	VK_CHECK_THROW(
		vkCreateInstance(&createInfo, VK_NULL_HANDLE, &instance_),
		"failed to create instance!"
	);
}

void UContext::SetupDebugMessenger()
{
	if constexpr (!ENABLE_VALIDATION_LAYERS)
	{
		return;
	}

	auto const createInfo{ getDebugMessengerCreateInfo() };

	VK_CHECK_THROW(
		createDebugUtilsMessengerEXT(instance_, &createInfo, nullptr, &debugMessenger_),
		"failed to set up debug messenger!"
	);
}
