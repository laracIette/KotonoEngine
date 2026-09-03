#include "Device.h"

#include "AllocatedBuffer.h"
#include "AllocatedImage.h"
#include "Context.h"
#include "Surface.h"
#include "vk_utils.h"
#include <algorithm>
#include <array>
#include <bit>
#include <kotono_common/log.h>
#include <ranges>
#include <Windows.h>

#define KT_LOG_IMPORTANCE_LEVEL_VMA ELogImportanceLevel::Low

static constexpr std::array DEVICE_EXTENSIONS
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

static b8 isComputerPluggedIn()
{
	SYSTEM_POWER_STATUS powerStatus;
	return GetSystemPowerStatus(&powerStatus)
		&& powerStatus.ACLineStatus == AC_LINE_ONLINE;
}

static UQueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	u32 queueFamilyCount{ 0 };
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies{ queueFamilyCount };
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	UQueueFamilyIndices indices{};

	u32 i{ 0 };
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport{ false };
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

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

static b8 checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	u32 extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions{ extensionCount };
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	return std::ranges::all_of(DEVICE_EXTENSIONS, [availableExtensions](std::string_view required) {
		return std::ranges::contains(availableExtensions
			| std::views::transform(&VkExtensionProperties::extensionName)
			//| std::views::transform([](char const* name) { return std::string_view{ name }; })
			, required
		);
	});
}

static b8 checkDeviceFeatureSupport(VkPhysicalDevice device)
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

static USwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	USwapchainSupportDetails details{};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	u32 formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	u32 presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

static b8 isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	UQueueFamilyIndices const indices{ findQueueFamilies(device, surface) };

	b8 const extensionsSupported{ checkDeviceExtensionSupport(device) };
	b8 const featuresSupported{ checkDeviceFeatureSupport(device) };

	bool swapchainAdequate{ false };
	if (extensionsSupported)
	{
		USwapchainSupportDetails const swapchainSupport{ querySwapchainSupport(device, surface) };
		swapchainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
	}

	return indices.IsComplete()
		&& extensionsSupported
		&& swapchainAdequate
		&& featuresSupported;
}

UDevice::UDevice(UContext& context, USurface& surface)
	: context_{ context }
	, surface_{ surface }
{
}

void UDevice::Init()
{
	CreatePhysicalDevice();
	CreateDevice();
	CreateAllocator();
	CreateCommandPool();

	CreateSingleTimeCommandBuffer();
	BeginSingleTimeCommandBuffer();

	PopulateSupportedMipmapFormats();

	depthFormat_ = FindSupportedDepthFormat();
	maxSamplerAnisotropy_ = FindMaxSamplerAnisotropy();
	msaaSamples_ = GetMaxUsableSampleCount();
}

void UDevice::Cleanup() const
{
	EndSingleTimeCommandBuffer();

	vkDestroyCommandPool(device_, commandPool_, nullptr);

	if constexpr (KT_SHOULD_LOG(KT_LOG_IMPORTANCE_LEVEL_VMA))
	{
		VmaTotalStatistics stats{};
		vmaCalculateStatistics(allocator_, &stats);

		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_VMA, "Platform", "VMA Allocator Stats:");
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_VMA, "Platform", "Total memory allocated: {0} bytes", stats.total.statistics.allocationBytes);
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_VMA, "Platform", "Number of allocations: {0}", stats.total.statistics.allocationCount);

		char* statsString;
		vmaBuildStatsString(allocator_, &statsString, true);
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_VMA, "Platform", "VMA Stats:\n{0}", statsString);
		vmaFreeStatsString(allocator_, statsString);
	}

	vmaDestroyAllocator(allocator_);

	vkDestroyDevice(device_, nullptr);
}

void UDevice::ExecuteSingleTimeCommands()
{
	EndSingleTimeCommandBuffer();

	VkCommandBufferSubmitInfo const cmdBufSubmitInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
		.commandBuffer = singleTimeCommandBuffer_,
	};

	VkSubmitInfo2 const submitInfo{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &cmdBufSubmitInfo,
	};
	VK_CHECK_THROW(
		vkQueueSubmit2(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE),
		"failed to submit the graphics queue!"
	);
	VK_CHECK_THROW(
		vkQueueWaitIdle(graphicsQueue_),
		"failed to wait for the graphics queue!"
	);

	ClearDeletionQueue();

	BeginSingleTimeCommandBuffer();
}

void UDevice::CopyBuffer(UAllocatedBuffer const& srcBuffer, UAllocatedBuffer const& dstBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size) const
{
	srcBuffer.CmdCopyTo(singleTimeCommandBuffer_, dstBuffer, srcOffset, dstOffset, size);
}

void UDevice::CopyBufferToImage(UAllocatedBuffer const& srcBuffer, UAllocatedImage const& dstImage, u32 width, u32 height) const
{
	VkBufferImageCopy2 const region{
		.sType = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2,
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

	VkCopyBufferToImageInfo2 const copyBufInfo{
		.sType = VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2,
		.srcBuffer = srcBuffer.buffer,
		.dstImage = dstImage.image,
		.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		.regionCount = 1,
		.pRegions = &region,
	};

	vkCmdCopyBufferToImage2(singleTimeCommandBuffer_, &copyBufInfo);
}

void UDevice::GenerateMipmaps(UAllocatedImage const& image, VkFormat format, i32 width, i32 height, u32 mipLevels) const
{
	if (!supportedMipmapFormats_.contains(format))
	{
		throw std::invalid_argument{ "unsupported image format for mipmap generation!" };
	}

	image.GenerateMipmaps(singleTimeCommandBuffer_, format, width, height, mipLevels);
}

void UDevice::TransitionImageLayout(UAllocatedImage const& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, u32 mipLevels) const
{
	image.TransitionLayout(singleTimeCommandBuffer_, format, oldLayout, newLayout, mipLevels);
}

void UDevice::StageBufferForDeletion(UAllocatedBuffer const& allocatedBuffer)
{
	deletionQueue_.push_back(allocatedBuffer);
}

void UDevice::StagingUpload(void const* data, VkDeviceSize size, UAllocatedBuffer const& dstBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset)
{
	// Create a temporary host-visible staging buffer
	UAllocatedBuffer stagingBuffer{};
	stagingBuffer.Create(device_, allocator_
		, size
		, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		, VMA_ALLOCATION_CREATE_MAPPED_BIT
		| VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
	);

	// Copy CPU data into the staging buffer
	std::memcpy(stagingBuffer.allocationInfo.pMappedData, data, size);

	// Flush manually to prevent bugs
	VK_CHECK_THROW(
		vmaFlushAllocation(allocator_, stagingBuffer.allocation, 0, VK_WHOLE_SIZE),
		"failed to flush VMA allocation!"
	);
	CopyBuffer(stagingBuffer, dstBuffer, srcOffset, dstOffset, size);

	//// Make the copy visible to shaders
	//VkBufferMemoryBarrier2 const barrier{
	//	.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2,
	//	.srcStageMask = VK_PIPELINE_STAGE_2_COPY_BIT,
	//	.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
	//	.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
	//	.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT,
	//	.buffer = dstBuffer.buffer,
	//	.offset = dstOffset,
	//	.size = size,
	//};
	//VkDependencyInfo const depInfo{
	//	.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
	//	.bufferMemoryBarrierCount = 1,
	//	.pBufferMemoryBarriers = &barrier,
	//};
	//vkCmdPipelineBarrier2(singleTimeCommandBuffer_, &depInfo);

	StageBufferForDeletion(stagingBuffer);
}

UAllocatedBuffer UDevice::CreateAllocatedBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateFlags allocFlags, VmaMemoryUsage memUsage) const
{
	UAllocatedBuffer allocatedBuffer{};
	allocatedBuffer.Create(device_, allocator_, size, usage, allocFlags, memUsage);
	return allocatedBuffer;
}

UAllocatedImage UDevice::CreateAllocatedImage(UAllocatedImageCreateInfo const& createInfo) const
{
	UAllocatedImage allocatedImage{};
	allocatedImage.Create(device_, allocator_, createInfo);
	return allocatedImage;
}

void UDevice::CreateImage(UAllocatedImage& allocatedImage, UAllocatedImageCreateInfo const& createInfo) const
{
	allocatedImage.CreateImage(allocator_, createInfo);
}

void UDevice::CreateImageView(UAllocatedImage& allocatedImage, UAllocatedImageCreateInfo const& createInfo) const
{
	allocatedImage.CreateImageView(device_, createInfo);
}

void UDevice::CleanupAllocatedBuffer(UAllocatedBuffer const& allocatedBuffer) const
{
	allocatedBuffer.Cleanup(allocator_);
}

void UDevice::CleanupAllocatedImage(UAllocatedImage const& allocatedImage) const
{
	allocatedImage.Cleanup(device_, allocator_);
}

void UDevice::CleanupImage(UAllocatedImage const& allocatedImage) const
{
	allocatedImage.CleanupImage(allocator_);
}

void UDevice::CleanupImageView(UAllocatedImage const& allocatedImage) const
{
	allocatedImage.CleanupImageView(device_);
}

USwapchainSupportDetails UDevice::QuerySwapchainSupportDetails() const
{
	return querySwapchainSupport(physicalDevice_, surface_.GetSurface());
}

void UDevice::CreatePhysicalDevice()
{
	u32 deviceCount;
	vkEnumeratePhysicalDevices(context_.GetInstance(), &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error{ "failed to find GPUs with Vulkan support!" };
	}

	std::vector<VkPhysicalDevice> devices{ deviceCount };
	vkEnumeratePhysicalDevices(context_.GetInstance(), &deviceCount, devices.data());

	VkPhysicalDevice bestDevice{ VK_NULL_HANDLE };
	VkDeviceSize maxVRAM{ 0 };

	for (auto const& device : devices)
	{
		if (isDeviceSuitable(device, surface_.GetSurface()))
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
			if (!isComputerPluggedIn())
			{
				break;
			}
		}
	}

	if (bestDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error{ "failed to find a suitable GPU!" };
	}

	physicalDevice_ = bestDevice;
	queueFamilyIndices_ = findQueueFamilies(bestDevice, surface_.GetSurface());
}

void UDevice::CreateDevice()
{
	std::unordered_set const uniqueQueueFamilies{ queueFamilyIndices_.graphicsFamily.value(), queueFamilyIndices_.presentFamily.value() };

	constexpr f32 QUEUE_PRIORITY{ 1.0f };

	auto const queueCreateInfos{ uniqueQueueFamilies
		| std::views::transform([QUEUE_PRIORITY](u32 queueFamily) {
			return VkDeviceQueueCreateInfo{
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.queueFamilyIndex = queueFamily,
				.queueCount = 1,
				.pQueuePriorities = &QUEUE_PRIORITY,
			};
		})
		| std::ranges::to<std::vector>()
	};

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

	VkPhysicalDeviceFeatures2 const features2{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext = &features11,
		.features = {
			.sampleRateShading = VK_TRUE,
			.fillModeNonSolid = VK_TRUE, // enable wireframe
			.samplerAnisotropy = VK_TRUE,
		},
	};

	VkDeviceCreateInfo const createInfo{
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

	vkGetDeviceQueue(device_, queueFamilyIndices_.graphicsFamily.value(), 0, &graphicsQueue_);
	vkGetDeviceQueue(device_, queueFamilyIndices_.presentFamily.value(), 0, &presentQueue_);
}

void UDevice::CreateAllocator()
{
	VmaAllocatorCreateInfo const allocatorInfo{
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = physicalDevice_,
		.device = device_,
		.instance = context_.GetInstance(),
		.vulkanApiVersion = VK_API_VERSION_1_4,
	};
	VK_CHECK_THROW(
		vmaCreateAllocator(&allocatorInfo, &allocator_),
		"Failed to create VMA allocator!"
	);
}

void UDevice::CreateCommandPool()
{
	const VkCommandPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = queueFamilyIndices_.graphicsFamily.value(),
	};

	VK_CHECK_THROW(
		vkCreateCommandPool(device_, &poolInfo, nullptr, &commandPool_),
		"failed to create command pool!"
	);
}

void UDevice::CreateSingleTimeCommandBuffer()
{
	VkCommandBufferAllocateInfo const allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = commandPool_,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};

	VK_CHECK_THROW(
		vkAllocateCommandBuffers(device_, &allocInfo, &singleTimeCommandBuffer_),
		"failed to allocate command buffer!"
	);
}

void UDevice::BeginSingleTimeCommandBuffer() const
{
	VkCommandBufferBeginInfo const beginInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};
	VK_CHECK_THROW(
		vkBeginCommandBuffer(singleTimeCommandBuffer_, &beginInfo),
		"failed to begin single time command buffer!"
	);
}

void UDevice::EndSingleTimeCommandBuffer() const
{
	VK_CHECK_THROW(
		vkEndCommandBuffer(singleTimeCommandBuffer_),
		"failed to end single time command buffer!"
	);
}

void UDevice::ClearDeletionQueue()
{
	for (auto const& stagingBuffer : deletionQueue_)
	{
		stagingBuffer.Cleanup(allocator_);
	}
	deletionQueue_.clear();
}

b8 UDevice::FormatSupported(VkFormat format, VkFormatFeatureFlags features) const
{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(physicalDevice_, format, &formatProperties);

	return (formatProperties.optimalTilingFeatures & features) == features;
}

void UDevice::PopulateSupportedMipmapFormats()
{
	constexpr std::array CANDIDATES{ VK_FORMAT_R8G8B8A8_SRGB };

	supportedMipmapFormats_.insert_range(CANDIDATES
		| std::views::filter([this](VkFormat format) {
			return FormatSupported(format, VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT);
		})
	);
}

VkFormat UDevice::FindSupportedDepthFormat() const
{
	constexpr std::array CANDIDATES{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };

	auto const it{ std::ranges::find_if(CANDIDATES
		, [this](VkFormat format) {
			return FormatSupported(format, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
		}
	) };

	if (it == CANDIDATES.end())
	{
		throw std::runtime_error{ "couldn't find a supported depth format!" };
	}

	return *it;
}

f32 UDevice::FindMaxSamplerAnisotropy() const
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(physicalDevice_, &properties);
	return properties.limits.maxSamplerAnisotropy;
}

VkSampleCountFlagBits UDevice::GetMaxUsableSampleCount() const
{
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice_, &physicalDeviceProperties);

	const VkSampleCountFlags counts{ physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts };
	return static_cast<VkSampleCountFlagBits>(std::bit_floor(counts));
}
