#include "Context.h"
#include "vk_utils.h"
#include "Window.h"
#include <array>
#include <glm/common.hpp>
#include <kotono_common/log.h>
#include <print>
#include <set>

#define KT_LOG_IMPORTANCE_LEVEL_VMA ELogImportanceLevel::Low

static constexpr std::array VALIDATION_LAYERS
{
	"VK_LAYER_KHRONOS_validation",
};

static constexpr std::array VALIDATION_FEATURES
{
	VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT,
};

static constexpr std::array DEVICE_EXTENSIONS
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

#ifdef NDEBUG
static constexpr bool ENABLE_VALIDATION_LAYERS{ false };
#else
static constexpr bool ENABLE_VALIDATION_LAYERS{ true };
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	std::println("[Vulkan Validation Layer] {0}", pCallbackData->pMessage);
	return VK_FALSE;
}

void GContext::Init()
{
	CreateInstance();
	SetupDebugMessenger();
	CreateSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateAllocator();
	CreateCommandPool();
}

void GContext::Cleanup()
{
	KT_LOG(ELogImportanceLevel::High, "Platform", "cleaning up context");

	vkDestroyCommandPool(device_, commandPool_, nullptr);

	if constexpr (KT_SHOULD_LOG(KT_LOG_IMPORTANCE_LEVEL_VMA))
	{
		VmaTotalStatistics stats{};
		vmaCalculateStatistics(allocator_, &stats);

		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_VMA, "Platform", "VMA Allocator Stats:");
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_VMA, "Platform", "Total memory allocated: {} bytes", stats.total.statistics.allocationBytes);
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_VMA, "Platform", "Number of allocations: {}", stats.total.statistics.allocationCount);

		char* statsString;
		vmaBuildStatsString(allocator_, &statsString, true);
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_VMA, "Platform", "VMA Stats:\n{}", statsString);
		vmaFreeStatsString(allocator_, statsString);
	}

	vmaDestroyAllocator(allocator_);

	vkDestroyDevice(device_, nullptr);

	if constexpr (ENABLE_VALIDATION_LAYERS)
	{
		DestroyDebugUtilsMessengerEXT(instance_, debugMessenger_, nullptr);
	}

	vkDestroySurfaceKHR(instance_, surface_, nullptr);
	vkDestroyInstance(instance_, nullptr);

	KT_LOG(ELogImportanceLevel::High, "Platform", "cleaned up context");
}

void GContext::CreateInstance()
{
	if (ENABLE_VALIDATION_LAYERS && !CheckValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}

	const VkApplicationInfo appInfo{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Kotono Engine Application",
		.applicationVersion = VK_MAKE_VERSION(0, 0, 1),
		.pEngineName = "Kotono Engine",
		.engineVersion = VK_MAKE_VERSION(0, 0, 1),
		.apiVersion = VK_API_VERSION_1_4,
	};

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	VkValidationFeaturesEXT validationFeatures{};
	const void* pNext;
	u32 enabledLayerCount;

	if constexpr (ENABLE_VALIDATION_LAYERS)
	{
		validationFeatures = {
			.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
			.pNext = &debugCreateInfo,
			.enabledValidationFeatureCount = static_cast<u32>(VALIDATION_FEATURES.size()),
			.pEnabledValidationFeatures = VALIDATION_FEATURES.data(),
		};

		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		pNext = &validationFeatures;
		enabledLayerCount = static_cast<u32>(VALIDATION_LAYERS.size());
	}
	else
	{
		pNext = VK_NULL_HANDLE;
		enabledLayerCount = 0;
	}

	const auto extensions{ GetRequiredExtensions() };

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

void GContext::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const
{
	createInfo = {
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.pNext = VK_NULL_HANDLE,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = debugCallback,
	};
}

void GContext::SetupDebugMessenger()
{
	if constexpr (!ENABLE_VALIDATION_LAYERS)
	{
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	PopulateDebugMessengerCreateInfo(createInfo);

	VK_CHECK_THROW(
		CreateDebugUtilsMessengerEXT(instance_, &createInfo, nullptr, &debugMessenger_),
		"failed to set up debug messenger!"
	);
}

bool GContext::CheckValidationLayerSupport()
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

std::vector<const char*> GContext::GetRequiredExtensions()
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

void GContext::PickPhysicalDevice()
{
	u32 deviceCount;
	vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices{ deviceCount };
	vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

	VkPhysicalDevice bestDevice{ VK_NULL_HANDLE };
	VkDeviceSize maxVRAM{ 0 };

	for (const auto& device : devices)
	{
		if (IsDeviceSuitable(device))
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			// Get the memory properties of the GPU
			VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

			VkDeviceSize totalVRAM{ 0 };
			// Iterate over memory types and sum the VRAM of the suitable types
			for (u32 i{ 0 }; i < memoryProperties.memoryTypeCount; ++i)
			{
				const VkMemoryType& memoryType = memoryProperties.memoryTypes[i];
				// Consider only the VRAM (local memory)
				if (memoryType.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
				{
					totalVRAM += memoryProperties.memoryHeaps[memoryProperties.memoryTypes[i].heapIndex].size;
				}
			}

			// If this device has more VRAM than the previously selected one
			if (totalVRAM > maxVRAM)
			{
				maxVRAM = totalVRAM;
				bestDevice = device;

				KT_LOG(ELogImportanceLevel::High, "Platform", "Selected GPU: {}, VRAM: {} MB", deviceProperties.deviceName, totalVRAM / (1024llu * 1024));
			}

			// Stop on cpu if on battery
			if (!GetIsComputerPluggedIn())
			{
				break;
			}
		}
	}

	if (bestDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	physicalDevice_ = bestDevice;
	msaaSamples_ = GetMaxUsableSampleCount();
}

bool GContext::IsDeviceSuitable(VkPhysicalDevice device)
{
	const KtQueueFamilyIndices indices{ FindQueueFamilies(device) };

	const bool extensionsSupported{ CheckDeviceExtensionSupport(device) };
	const bool featuresSupported{ CheckDeviceFeatureSupport(device) };

	bool swapChainAdequate{ false };
	if (extensionsSupported)
	{
		const KtSwapChainSupportDetails swapChainSupport{ QuerySwapChainSupport(device) };
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.IsComplete() 
		&& extensionsSupported 
		&& swapChainAdequate 
		&& featuresSupported;
}

bool GContext::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	u32 extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions{ extensionCount };
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions{ DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end() };

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool GContext::CheckDeviceFeatureSupport(VkPhysicalDevice device)
{
	VkPhysicalDeviceVulkan14Features features14{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES,
		.pNext = VK_NULL_HANDLE,
	};
	VkPhysicalDeviceVulkan13Features features13{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.pNext = &features14,
	};
	VkPhysicalDeviceVulkan12Features features12{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		.pNext = &features13,
	};
	VkPhysicalDeviceVulkan11Features features11{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
		.pNext = &features12,
	};
	VkPhysicalDeviceFeatures2 features2{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext = &features11,
	};
	vkGetPhysicalDeviceFeatures2(device, &features2);

	const bool features2Supported{ features2.features.samplerAnisotropy
		&& features2.features.fillModeNonSolid
		&& features2.features.sampleRateShading
	};

	const bool features11Supported{ features11.multiview 
		&& features11.shaderDrawParameters
	};

	const bool features12Supported{ features12.shaderSampledImageArrayNonUniformIndexing
		&& features12.shaderStorageBufferArrayNonUniformIndexing
		&& features12.descriptorBindingSampledImageUpdateAfterBind 
		&& features12.descriptorBindingStorageImageUpdateAfterBind 
		&& features12.descriptorBindingStorageBufferUpdateAfterBind 
		&& features12.descriptorBindingPartiallyBound
		&& features12.descriptorBindingVariableDescriptorCount
		&& features12.runtimeDescriptorArray
		&& features12.scalarBlockLayout
		&& features12.bufferDeviceAddress
	};

	const bool features13Supported{ features13.dynamicRendering
		&& features13.synchronization2 
	};

	const bool features14Supported{ features14.pushDescriptor == VK_TRUE };

    return features2Supported 
		&& features11Supported
		&& features12Supported 
		&& features13Supported
		&& features14Supported;
}

KtQueueFamilyIndices GContext::FindQueueFamilies(VkPhysicalDevice device) const
{
	KtQueueFamilyIndices indices{};

	u32 queueFamilyCount{ 0 };
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies{ queueFamilyCount };
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	u32 i{ 0 };
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport{ false };
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.IsComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

void GContext::CreateLogicalDevice()
{
	const KtQueueFamilyIndices indices{ FindQueueFamilies(physicalDevice_) };

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
	const std::set uniqueQueueFamilies{ indices.graphicsFamily.value(), indices.presentFamily.value() };

	const float queuePriority{ 1.0f };
	for (u32 queueFamily : uniqueQueueFamilies)
	{
		const VkDeviceQueueCreateInfo queueCreateInfo{
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = queueFamily,
			.queueCount = 1,
			.pQueuePriorities = &queuePriority,
		};
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceVulkan14Features features14{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES,
		.pNext = VK_NULL_HANDLE,
		.pushDescriptor = VK_TRUE,
	};

	VkPhysicalDeviceVulkan13Features features13{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.pNext = &features14,
		.synchronization2 = VK_TRUE,
		.dynamicRendering = VK_TRUE,
	};

	VkPhysicalDeviceVulkan12Features features12{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		.pNext = &features13,
		// Descriptor indexing
		.shaderSampledImageArrayNonUniformIndexing = VK_TRUE,
		.shaderStorageBufferArrayNonUniformIndexing = VK_TRUE,
		.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE,
		.descriptorBindingStorageImageUpdateAfterBind = VK_TRUE,
		.descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE,
		.descriptorBindingPartiallyBound = VK_TRUE,
		.descriptorBindingVariableDescriptorCount = VK_TRUE,
		.runtimeDescriptorArray = VK_TRUE,
		// BDA
		.scalarBlockLayout = VK_TRUE,
		.bufferDeviceAddress = VK_TRUE,
	};

	VkPhysicalDeviceVulkan11Features features11{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
		.pNext = &features12,
		.multiview = VK_TRUE,
		.shaderDrawParameters = VK_TRUE,
	};

	const VkPhysicalDeviceFeatures2 features2{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext = &features11,
		.features = {
			.sampleRateShading = VK_TRUE,
			.fillModeNonSolid = VK_TRUE, // enable wireframe
			.samplerAnisotropy = VK_TRUE,
		},
	};

	const VkDeviceCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = &features2,
		
		.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size()),
		.pQueueCreateInfos = queueCreateInfos.data(),
		
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = VK_NULL_HANDLE,
		
		.enabledExtensionCount = static_cast<u32>(DEVICE_EXTENSIONS.size()),
		.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data(),
	};

	VK_CHECK_THROW(
		vkCreateDevice(physicalDevice_, &createInfo, nullptr, &device_),
		"failed to create logical device!"
	);

	vkGetDeviceQueue(device_, indices.graphicsFamily.value(), 0, &graphicsQueue_);
	vkGetDeviceQueue(device_, indices.presentFamily.value(), 0, &presentQueue_);
}

void GContext::CreateSurface()
{
	VK_CHECK_THROW(
		glfwCreateWindowSurface(instance_, Window.GetGLFWWindow(), nullptr, &surface_),
		"failed to create window surface!"
	);
}

KtSwapChainSupportDetails GContext::QuerySwapChainSupport(VkPhysicalDevice device) const
{
	KtSwapChainSupportDetails details{};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, &details.capabilities);

	u32 formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, details.formats.data());
	}

	u32 presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, details.presentModes.data());
	}

	return details;
}

VkResult GContext::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const
{
	const auto func{ (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT") };
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void GContext::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) const
{
	const auto func{ (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT") };
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

VkSurfaceFormatKHR GContext::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
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

VkPresentModeKHR GContext::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const
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

VkExtent2D GContext::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
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

void GContext::CreateAllocator()
{
	const VmaAllocatorCreateInfo allocatorInfo{
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = physicalDevice_,
		.device = device_,
		.instance = instance_,
		.vulkanApiVersion = VK_API_VERSION_1_4,
	};
	VK_CHECK_THROW(
		vmaCreateAllocator(&allocatorInfo, &allocator_),
		"Failed to create VMA allocator"
	);
}

VkCommandBuffer GContext::BeginSingleTimeCommands() const
{
	const VkCommandBufferAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = commandPool_,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	VkCommandBuffer commandBuffer{};
	VK_CHECK_THROW(
		vkAllocateCommandBuffers(device_, &allocInfo, &commandBuffer),
		"failed to allocate command buffer!"
	);

	const VkCommandBufferBeginInfo beginInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};
	VK_CHECK_THROW(
		vkBeginCommandBuffer(commandBuffer, &beginInfo),
		"failed to begin single time command buffer"
	);

	return commandBuffer;
}

void GContext::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer),
		"failed to end single time command buffer"
	);
	singleTimeCommands_.push_back(commandBuffer);
}

void GContext::ExecuteSingleTimeCommands()
{
	if (singleTimeCommands_.empty())
	{
		return;
	}

	const VkSubmitInfo submitInfo{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = static_cast<u32>(singleTimeCommands_.size()),
		.pCommandBuffers = singleTimeCommands_.data(),
	};
	vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue_);

	vkFreeCommandBuffers(device_, commandPool_, static_cast<u32>(singleTimeCommands_.size()), singleTimeCommands_.data());

	singleTimeCommands_.clear();

	eventExecuteSingleTimeCommands_.Broadcast();
	eventExecuteSingleTimeCommands_.Clear();
}

UEvent<>& GContext::GetEventExecuteSingleTimeCommands()
{
	return eventExecuteSingleTimeCommands_;
}

void GContext::StagingUpload(const void* data
	, const VkDeviceSize dataSize
	, VkBuffer dstBuffer
	, const VkDeviceSize dstOffset)
{
	// Create a temporary host-visible staging buffer
	UAllocatedBuffer stagingBuffer;
	CreateBuffer(stagingBuffer
		, dataSize
		, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
		, VMA_MEMORY_USAGE_AUTO
	);

	// Copy CPU data into the staging buffer
	std::memcpy(stagingBuffer.allocationInfo.pMappedData, data, dataSize);

	// Flush manually to prevent bugs
	vmaFlushAllocation(allocator_, stagingBuffer.allocation, 0, VK_WHOLE_SIZE);

	VkCommandBuffer commandBuffer{ BeginSingleTimeCommands() };

	// Record the GPU-side copy
	const VkBufferCopy region{
		.srcOffset = 0,
		.dstOffset = dstOffset,
		.size = dataSize,
	};
	vkCmdCopyBuffer(commandBuffer, stagingBuffer.buffer, dstBuffer, 1, &region);

	// Make the copy visible to shaders
	const VkBufferMemoryBarrier2 barrier{
		.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2,
		.srcStageMask = VK_PIPELINE_STAGE_2_COPY_BIT,
		.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
		.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT,
		.buffer = dstBuffer,
		.offset = dstOffset,
		.size = dataSize,
	};
	const VkDependencyInfo depInfo{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.bufferMemoryBarrierCount = 1,
		.pBufferMemoryBarriers = &barrier,
	};
	vkCmdPipelineBarrier2(commandBuffer, &depInfo);

	EndSingleTimeCommands(commandBuffer);

	// Staging buffer must outlive the command buffer
	deletionQueue_.push_back(stagingBuffer);

	GetEventExecuteSingleTimeCommands().AddListener(this, &GContext::ClearDeletionQueue);
}

void GContext::CreateCommandPool()
{
	const KtQueueFamilyIndices queueFamilyIndices{ FindQueueFamilies(physicalDevice_) };

	const VkCommandPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(),
	};

	VK_CHECK_THROW(
		vkCreateCommandPool(device_, &poolInfo, nullptr, &commandPool_),
		"failed to create command pool!"
	);
}

void GContext::CreateBuffer(UAllocatedBuffer& allocatedBuffer
	, const VkDeviceSize size
	, const VkBufferUsageFlags usage
	, const VmaAllocationCreateFlags allocFlags
	, const VmaMemoryUsage memUsage) const
{
	const VkBufferCreateInfo bufInfo{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = size,
		.usage = usage,
	};

	const VmaAllocationCreateInfo allocInfo{
		.flags = allocFlags,
		.usage = memUsage,
	};

	VK_CHECK_THROW(
		vmaCreateBuffer(allocator_
			, &bufInfo
			, &allocInfo
			, &allocatedBuffer.buffer
			, &allocatedBuffer.allocation
			, &allocatedBuffer.allocationInfo
		),
		"failed to create buffer with VMA!"
	);

	if ((allocFlags & VMA_ALLOCATION_CREATE_MAPPED_BIT) && !allocatedBuffer.allocationInfo.pMappedData)
	{
		throw "Staging buffer was not mapped as expected!";
	}

	if (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
	{
		const VkBufferDeviceAddressInfo addrInfo{
			.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
			.buffer = allocatedBuffer.buffer,
		};
		allocatedBuffer.bda = vkGetBufferDeviceAddress(device_, &addrInfo);
	}
}

void GContext::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer{ BeginSingleTimeCommands() };

	const VkBufferCopy copyRegion{
		.size = size,
	};
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	EndSingleTimeCommands(commandBuffer);
}

u32 GContext::FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice_, &memProperties);

	for (u32 i{ 0 }; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void GContext::CreateImage(const u32 width, const u32 height
	, const u32 mipLevels
	, const u32 arrayLayers
	, const VkSampleCountFlagBits numSamples
	, const VkFormat format
	, const VkImageTiling tiling
	, const VkImageUsageFlags usage
	, const VkMemoryPropertyFlags properties
	, VkImage& image
	, VmaAllocation& imageAllocation) const
{
	const VkImageCreateInfo imageInfo{
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = format,
		.extent{
			.width = width,
			.height = height,
			.depth = 1,
		},
		.mipLevels = mipLevels,
		.arrayLayers = arrayLayers,
		.samples = numSamples,
		.tiling = tiling,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
	};

	const VmaAllocationCreateInfo allocCreateInfo{
		.usage = VMA_MEMORY_USAGE_AUTO,
		.requiredFlags = properties,
	};

	VK_CHECK_THROW(
		vmaCreateImage(allocator_, &imageInfo, &allocCreateInfo, &image, &imageAllocation, nullptr),
		"failed to create image with memory allocation!"
	);
}

VkFormat GContext::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice_, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

VkFormat GContext::FindDepthFormat() const
{
	return FindSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

bool GContext::HasStencilComponent(VkFormat format) const
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void GContext::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, u32 mipLevels)
{
	VkCommandBuffer commandBuffer{ BeginSingleTimeCommands() };

	VkImageMemoryBarrier barrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.oldLayout = oldLayout,
		.newLayout = newLayout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = image,
		.subresourceRange{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = mipLevels,
			.baseArrayLayer = 0,
			.layerCount = 1,
		},
	};
	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

		if (HasStencilComponent(format))
		{
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_NONE;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_NONE;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(commandBuffer
		, sourceStage, destinationStage
		, 0
		, 0, nullptr
		, 0, nullptr
		, 1, &barrier
	);

	EndSingleTimeCommands(commandBuffer);
}

void GContext::CopyBufferToImage(VkBuffer buffer, VkImage image, u32 width, u32 height)
{
	VkCommandBuffer commandBuffer{ BeginSingleTimeCommands() };

	const VkBufferImageCopy region{
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageSubresource{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.mipLevel = 0,
			.baseArrayLayer = 0,
			.layerCount = 1,
		},
		.imageOffset = { 0, 0, 0 },
		.imageExtent = { width, height, 1 },
	};

	vkCmdCopyBufferToImage(
		commandBuffer,
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	EndSingleTimeCommands(commandBuffer);
}

VkImageView GContext::CreateImageView(VkImage image
	, const VkImageViewType viewType
	, const VkFormat format
	, const VkImageAspectFlags aspectFlags
	, const u32 mipLevels
	, const u32 layerCount) const
{
	const VkImageViewCreateInfo viewInfo{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = image,
		.viewType = viewType,
		.format = format,
		.subresourceRange{
			.aspectMask = aspectFlags,
			.baseMipLevel = 0,
			.levelCount = mipLevels,
			.baseArrayLayer = 0,
			.layerCount = layerCount,
		},
	};

	VkImageView imageView;
	VK_CHECK_THROW(
		vkCreateImageView(device_, &viewInfo, nullptr, &imageView),
		"failed to create texture image view!"
	);

	return imageView;
}

void GContext::CreateSampledImageAndImageView(UAllocatedImage& allocatedImage
	, const VkExtent2D extent
	, const u32 arrayLayers
	, const VkFormat format
	, const VkImageUsageFlagBits usage
	, const VkImageViewType viewType
	, const VkImageAspectFlagBits aspect) const
{
	CreateImage(extent.width, extent.height
		, 1
		, arrayLayers
		, VK_SAMPLE_COUNT_1_BIT
		, format
		, VK_IMAGE_TILING_OPTIMAL
		, VK_IMAGE_USAGE_SAMPLED_BIT
		| usage
		, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		, allocatedImage.image
		, allocatedImage.allocation
	);

	allocatedImage.imageView = CreateImageView(allocatedImage.image
		, viewType
		, format
		, aspect
		, 1
		, arrayLayers
	);
}

void GContext::GenerateMipmaps(VkImage image, VkFormat imageFormat, i32 texWidth, i32 texHeight, u32 mipLevels)
{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(physicalDevice_, imageFormat, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
	{
		throw std::runtime_error("texture image format does not support linear blitting!");
	}

	VkCommandBuffer commandBuffer{ BeginSingleTimeCommands() };

	VkImageMemoryBarrier barrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = image,
		.subresourceRange{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1,
		},
	};

	i32 mipWidth = texWidth;
	i32 mipHeight = texHeight;

	for (u32 i{ 1 }; i < mipLevels; i++)
	{
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		const VkImageBlit blit{
			.srcSubresource{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = i - 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
			.srcOffsets = { 
				{ 0, 0, 0 },
				{ mipWidth, mipHeight, 1 } 
			},
			.dstSubresource{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = i,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
			.dstOffsets = { 
				{ 0, 0, 0 },
				{ mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 }
			},
		};

		vkCmdBlitImage(commandBuffer,
			image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR
		);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	EndSingleTimeCommands(commandBuffer);
}

VkSampleCountFlagBits GContext::GetMaxUsableSampleCount() const
{
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice_, &physicalDeviceProperties);

	const VkSampleCountFlags counts{ physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts };
	if (counts & VK_SAMPLE_COUNT_64_BIT) return VK_SAMPLE_COUNT_64_BIT; 
	if (counts & VK_SAMPLE_COUNT_32_BIT) return VK_SAMPLE_COUNT_32_BIT; 
	if (counts & VK_SAMPLE_COUNT_16_BIT) return VK_SAMPLE_COUNT_16_BIT; 
	if (counts & VK_SAMPLE_COUNT_8_BIT) return VK_SAMPLE_COUNT_8_BIT; 
	if (counts & VK_SAMPLE_COUNT_4_BIT) return VK_SAMPLE_COUNT_4_BIT; 
	if (counts & VK_SAMPLE_COUNT_2_BIT) return VK_SAMPLE_COUNT_2_BIT; 
	else return VK_SAMPLE_COUNT_1_BIT;
}


VkPhysicalDevice& GContext::GetPhysicalDevice()
{
	return physicalDevice_;
}

VkDevice& GContext::GetDevice()
{
	return device_;
}

VmaAllocator& GContext::GetAllocator() 
{ 
	return allocator_; 
}

VkQueue& GContext::GetGraphicsQueue()
{
	return graphicsQueue_;
}

VkQueue& GContext::GetPresentQueue()
{
	return presentQueue_;
}

VkSurfaceKHR& GContext::GetSurface()
{
	return surface_;
}

VkSampleCountFlagBits GContext::GetMSAASamples() const
{
	return msaaSamples_;
}

bool GContext::GetIsComputerPluggedIn()
{
	SYSTEM_POWER_STATUS powerStatus;
	return GetSystemPowerStatus(&powerStatus) 
		&& powerStatus.ACLineStatus == AC_LINE_ONLINE;
}

void GContext::ClearDeletionQueue()
{
	if (deletionQueue_.empty())
	{
		return;
	}

	for (auto& stagingBuffer : deletionQueue_)
	{
		vmaDestroyBuffer(allocator_, stagingBuffer.buffer, stagingBuffer.allocation);
	}
	deletionQueue_.clear();
}
