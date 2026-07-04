#include "Renderer.h"
#include "Color.h"
#include "DrawCall.h"
#include "DrawDataBuffer.h"
#include "LightBuffer.h"
#include "MaterialBuffer.h"
#include "ParametersBuffer.h"
#include "PipelineResourceManager.h"
#include "TransformBuffer.h"
#include <algorithm>
#include <kotono_platform/Context.h>
#include <kotono_platform/Window.h>
#include <kotono_platform/WindowViewport.h>
#include <kotono_common/log.h>
#include <kotono_platform/glm_utils.h>
#include <kotono_platform/vk_utils.h>
#include <ranges>
#include <tuple>

void GRenderer::Init()
{
	CreateSwapChain();
	CreateImageViews();
	CreateColorResources();
	CreateDepthResources();
	CreateCommandPools();
	CreateCommandBuffers();
	CreateSyncObjects();

	PipelineResourceManager.Init();
	DrawDataBuffer.Init();
	MaterialBuffer.Init();
	TransformBuffer.Init();
	ParametersBuffer.Init();
	LightBuffer.Init();

	LightBuffer.RegisterLight({
		.direction = glm::normalize(-WorldForwardVector - WorldUpVector * 0.5f),
		.color = Colors::White,
		.intensity = 1.0f,
		.type = static_cast<u32>(ELightType::Directional),
	});

	LightBuffer.RegisterLight({
		.position = WorldUpVector + WorldForwardVector * 3.0f,
		.color = UColor::Mix(Colors::Green, Colors::Blue),
		.intensity = 100.0f,
		.range = 100.0f,
		.type = static_cast<u32>(ELightType::Point),
	});
}

void GRenderer::Cleanup()
{
	KT_LOG(ELogImportanceLevel::High, "Graphics", "cleaning up renderer");

	JoinThread(renderThread_);
	JoinThread(rhiThread_); 

	LightBuffer.Cleanup();
	ParametersBuffer.Cleanup();
	TransformBuffer.Cleanup();
	MaterialBuffer.Cleanup();
	DrawDataBuffer.Cleanup();
	PipelineResourceManager.Cleanup();

	CleanupSwapChain();

	for (auto& frameData : frameDatas_)
	{
		vkDestroySemaphore(Context.GetDevice(), frameData.renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(Context.GetDevice(), frameData.imageAvailableSemaphore, nullptr);
		vkDestroyFence(Context.GetDevice(), frameData.inFlightFence, nullptr);
		vkDestroyCommandPool(Context.GetDevice(), frameData.commandPool, nullptr);
	}

	KT_LOG(ELogImportanceLevel::High, "Graphics", "cleaned up renderer");
}

void GRenderer::CreateSwapChain()
{
	KtSwapChainSupportDetails swapChainSupport = Context.QuerySwapChainSupport(Context.GetPhysicalDevice());

	const auto surfaceFormat{ ChooseSwapSurfaceFormat(swapChainSupport.formats) };
	const auto presentMode{ ChooseSwapPresentMode(swapChainSupport.presentModes) };
	const auto extent{ ChooseSwapExtent(swapChainSupport.capabilities) };

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

	swapChainDatas_.resize(imageCount);
	for (size i{ 0 }; i < imageCount; ++i)
	{
		swapChainDatas_[i].image = swapChainImages[i];
	}

	swapChainFormat_ = surfaceFormat.format;
	swapChainExtent_ = extent;
}

VkSurfaceFormatKHR GRenderer::ChooseSwapSurfaceFormat(const std::span<VkSurfaceFormatKHR> availableFormats) const
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

VkPresentModeKHR GRenderer::ChooseSwapPresentMode(const std::span<VkPresentModeKHR> availablePresentModes) const
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

VkExtent2D GRenderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
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

void GRenderer::CreateImageViews()
{
	for (auto& swapChainData : swapChainDatas_)
	{
		swapChainData.imageView = Context.CreateImageView(swapChainData.image, swapChainFormat_, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}
}

void GRenderer::CreateColorResources()
{
	const VkFormat colorFormat{ swapChainFormat_ };

	for (auto& frameData : frameDatas_)
	{
		Context.CreateImage(
			swapChainExtent_.width,
			swapChainExtent_.height,
			1,
			Context.GetMSAASamples(),
			colorFormat,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			frameData.colorTarget.image,
			frameData.colorTarget.allocation
		);

		frameData.colorTarget.imageView = Context.CreateImageView(
			frameData.colorTarget.image,
			colorFormat,
			VK_IMAGE_ASPECT_COLOR_BIT,
			1
		);
	}
}

void GRenderer::CreateDepthResources()
{
	depthFormat_ = FindDepthFormat();
	
	for (auto& frameData : frameDatas_)
	{
		Context.CreateImage(
			swapChainExtent_.width,
			swapChainExtent_.height,
			1,
			Context.GetMSAASamples(),
			depthFormat_,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			frameData.depthTarget.image,
			frameData.depthTarget.allocation
		);

		frameData.depthTarget.imageView = Context.CreateImageView(
			frameData.depthTarget.image,
			depthFormat_,
			VK_IMAGE_ASPECT_DEPTH_BIT,
			1
		);

		Context.TransitionImageLayout(
			frameData.depthTarget.image,
			depthFormat_,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
			1
		);
	}
}

VkFormat GRenderer::FindSupportedFormat(const std::span<VkFormat> candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) const
{
	for (const VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(Context.GetPhysicalDevice(), format, &props);

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

VkFormat GRenderer::FindDepthFormat() const
{
	std::vector<VkFormat> formats{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
	return FindSupportedFormat(formats, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

void GRenderer::CreateCommandPools()
{
	for (size i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateCommandPool(static_cast<u32>(i));
	}
}

void GRenderer::CreateCommandPool(const u32 frameIndex)
{
	const KtQueueFamilyIndices queueFamilyIndices{ Context.FindQueueFamilies(Context.GetPhysicalDevice()) };

	const VkCommandPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(),
	};
	VK_CHECK_THROW(
		vkCreateCommandPool(Context.GetDevice(), &poolInfo, nullptr, &frameDatas_[frameIndex].commandPool),
		"failed to create command pool!"
	);
}

void GRenderer::CreateCommandBuffers()
{
	for (size i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateCommandBuffer(static_cast<u32>(i));
	}
}

void GRenderer::CreateCommandBuffer(const u32 frameIndex)
{
	const VkCommandBufferAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = frameDatas_[frameIndex].commandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Context.GetDevice(), &allocInfo, &frameDatas_[frameIndex].commandBuffer),
		"failed to allocate command buffers!"
	);
}

void GRenderer::CreateSyncObjects()
{
	const VkSemaphoreCreateInfo semaphoreInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};
	const VkFenceCreateInfo fenceInfo{
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT,
	};
	for (auto& frameData : frameDatas_)
	{
		if (vkCreateSemaphore(Context.GetDevice(), &semaphoreInfo, nullptr, &frameData.imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(Context.GetDevice(), &semaphoreInfo, nullptr, &frameData.renderFinishedSemaphore) != VK_SUCCESS ||
			vkCreateFence(Context.GetDevice(), &fenceInfo, nullptr, &frameData.inFlightFence) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}
static constexpr bool IS_MULTI_THREADED{ false };
void GRenderer::DrawFrame()
{
	const u32 frameIndex{ GetGameThreadFrame() };

	PipelineResourceManager.UpdateMappedFrameUBO(frameIndex);
	DrawDataBuffer.UpdateBuffer(frameIndex);
	TransformBuffer.UpdateBuffer(frameIndex);
	ParametersBuffer.UpdateBuffer(frameIndex);

	if constexpr (IS_MULTI_THREADED)
	{
		if (frameCount_ >= 1)
		{
			JoinThread(renderThread_);
			const u32 renderThreadFrame{ GetRenderThreadFrame() };
			renderThread_ = std::thread(&GRenderer::RecordCommandBuffer, this, renderThreadFrame);
		}

		if (frameCount_ >= 2)
		{
			KT_LOG(ELogImportanceLevel::High, "Graphics", "frame {0} rendered", frameCount_);

			JoinThread(rhiThread_);
			Context.ExecuteSingleTimeCommands();
			const u32 renderRHIFrame{ GetRHIThreadFrame() };
			rhiThread_ = std::thread(&GRenderer::SubmitCommandBuffer, this, renderRHIFrame);
		}
	}
	else
	{
		if (!TryAcquireNextImage(frameIndex))
		{
			KT_LOG(ELogImportanceLevel::High, "Graphics", "frame {0} skipped", frameCount_);
			return;
		}

		RecordCommandBuffer(frameIndex);
		Context.ExecuteSingleTimeCommands();
		SubmitCommandBuffer(frameIndex);
	}

	frameCount_++;
}

void GRenderer::RecordCommandBuffer(const u32 frameIndex)
{
	VkCommandBuffer commandBuffer{ frameDatas_[frameIndex].commandBuffer };
	vkResetCommandBuffer(commandBuffer, 0);

	BeginCommandBuffer(commandBuffer);

	CmdAcquireBarrier(commandBuffer, frameIndex);
	CmdBeginRendering(commandBuffer, frameIndex);

	PipelineResourceManager.CmdBindGlobalDescriptorSet(commandBuffer);
	PipelineResourceManager.CmdPushUniformDescriptorSet(commandBuffer, frameIndex);

	WindowViewport.CmdUse(commandBuffer);

	CmdDrawFrame(commandBuffer, frameIndex);

	CmdEndRendering(commandBuffer);
	CmdPresentationBarrier(commandBuffer, frameIndex);

	EndCommandBuffer(commandBuffer);
}

void GRenderer::BeginCommandBuffer(VkCommandBuffer commandBuffer)
{
	const VkCommandBufferBeginInfo beginInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	};
	VK_CHECK_THROW(
		vkBeginCommandBuffer(commandBuffer, &beginInfo),
		"failed to begin recording command buffer!"
	);
}

void GRenderer::CmdAcquireBarrier(VkCommandBuffer commandBuffer, const u32 frameIndex)
{
	const VkImageMemoryBarrier2 msaaTargetBarrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

		.srcStageMask = VK_PIPELINE_STAGE_2_NONE,
		.srcAccessMask = VK_ACCESS_2_NONE,

		.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,

		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

		.image = frameDatas_[frameIndex].colorTarget.image,
		.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	};

	const VkImageMemoryBarrier2 depthTargetBarrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

		.srcStageMask = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT
			| VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
		.srcAccessMask = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,

		.dstStageMask = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT
			| VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
		.dstAccessMask = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT
			| VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,

		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,

		.image = frameDatas_[frameIndex].depthTarget.image,
		.subresourceRange = { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1 }
	};

	const VkImageMemoryBarrier2 swapchainTargetBarrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

		.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.srcAccessMask = VK_ACCESS_2_NONE,

		.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,

		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

		.image = swapChainDatas_[frameDatas_[frameIndex].imageIndex].image,
		.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	};

	const std::array preRenderBarriers{ msaaTargetBarrier, depthTargetBarrier, swapchainTargetBarrier };
	const VkDependencyInfo preRenderDependencyInfo{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = static_cast<u32>(preRenderBarriers.size()),
		.pImageMemoryBarriers = preRenderBarriers.data(),
	};

	vkCmdPipelineBarrier2(commandBuffer, &preRenderDependencyInfo);
}

void GRenderer::CmdBeginRendering(VkCommandBuffer commandBuffer, const u32 frameIndex)
{
	const VkRenderingAttachmentInfo colorAttachment{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = frameDatas_[frameIndex].colorTarget.imageView,
		.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

		.resolveMode = VK_RESOLVE_MODE_AVERAGE_BIT,
		.resolveImageView = swapChainDatas_[frameDatas_[frameIndex].imageIndex].imageView, // The actual swapchain image for this frame
		.resolveImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.clearValue{
			.color = { 0.0f, 0.0f, 0.0f, 1.0f },
		},
	};

	const VkRenderingAttachmentInfo depthAttachment{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,

		.imageView = frameDatas_[frameIndex].depthTarget.imageView,
		.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,

		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.clearValue{
			.depthStencil = { 1.0f, 0 },
		},
	};

	const VkRenderingInfo renderInfo{
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea{
			.offset = { 0, 0 },
			.extent = swapChainExtent_
		},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachment,
		.pDepthAttachment = &depthAttachment,
		.pStencilAttachment = VK_NULL_HANDLE,
	};

	vkCmdBeginRendering(commandBuffer, &renderInfo);
}

void GRenderer::CmdDrawFrame(VkCommandBuffer commandBuffer, const u32 frameIndex) const
{
	auto sortedDrawCalls{ drawCalls_ };

	std::ranges::sort(sortedDrawCalls, std::less{}, [](const UDrawCall* dc) {
		return std::tie(dc->renderBucket, dc->sortKey, dc->pipeline);
	});

	//KT_LOG(KT_LOG_COMPILE_TIME_LEVEL, "Graphics", "{0}", drawCalls_.size());

	for (const auto* drawCall : sortedDrawCalls)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, drawCall->pipeline);

		const UPushConstants pc{
			.drawDataAddress = DrawDataBuffer.GetAddress(frameIndex),
			.materialAddress = MaterialBuffer.GetAddress(),
			.transformAddress = TransformBuffer.GetAddress(frameIndex),
			.parametersAddress = ParametersBuffer.GetAddress(frameIndex),
			.vertexBufferAddress = drawCall->vertexBufferAdress,
			.lightBufferAddress = LightBuffer.GetAddress(),
			.lightCount = LightBuffer.GetLightCount(),
			.drawIndex = drawCall->index,
		};

		vkCmdPushConstants(commandBuffer
			, PipelineResourceManager.GetPipelineLayout()
			, VK_SHADER_STAGE_ALL
			, 0
			, sizeof(UPushConstants)
			, &pc
		);

		vkCmdSetScissor(commandBuffer, 0, 1, &drawCall->scissor);

		vkCmdBindIndexBuffer(commandBuffer, drawCall->indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(commandBuffer
			, drawCall->indexCount
			, 1
			, drawCall->firstIndex
			, 0
			, 0
		);
	}
}

void GRenderer::CmdEndRendering(VkCommandBuffer commandBuffer)
{
	vkCmdEndRendering(commandBuffer);
}

void GRenderer::CmdPresentationBarrier(VkCommandBuffer commandBuffer, const u32 frameIndex)
{
	const VkImageMemoryBarrier2 presentBarrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

		.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,

		.dstStageMask = VK_PIPELINE_STAGE_2_NONE,
		.dstAccessMask = VK_ACCESS_2_NONE,

		.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,

		.image = swapChainDatas_[frameDatas_[frameIndex].imageIndex].image,
		.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
	};

	const VkDependencyInfo postRenderDependencyInfo{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &presentBarrier
	};

	vkCmdPipelineBarrier2(commandBuffer, &postRenderDependencyInfo);
}

void GRenderer::EndCommandBuffer(VkCommandBuffer commandBuffer)
{
	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer),
		"failed to record command buffer!"
	);
}

void GRenderer::RegisterDrawCall(UDrawCall* drawCall)
{
	if (!drawCall)
	{
		return;
	}

	drawCalls_.Add(drawCall);
	drawCall->poolIndex = drawCalls_.LastIndex();
}

void GRenderer::UnregisterDrawCall(UDrawCall* drawCall)
{
	if (!drawCall)
	{
		return;
	}

	const auto index{ drawCall->poolIndex };
	if (drawCalls_.RemoveAt(index) == EPoolRemoveResult::ItemSwappedAndRemoved)
	{
		drawCalls_[index]->poolIndex = index;
	}
}

void GRenderer::SubmitCommandBuffer(const u32 frameIndex)
{
	const VkCommandBufferSubmitInfo cmdBufInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
		.commandBuffer = frameDatas_[frameIndex].commandBuffer,
		.deviceMask = 0
	};

	const VkSemaphoreSubmitInfo waitSemaphoreInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
		.semaphore = frameDatas_[frameIndex].imageAvailableSemaphore,
		.value = 0, 
		.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.deviceIndex = 0,
	};

	const VkSemaphoreSubmitInfo signalSemaphoreInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
		.semaphore = frameDatas_[frameIndex].renderFinishedSemaphore,
		.value = 0,
		.stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
		.deviceIndex = 0,
	};

	const VkSubmitInfo2 submitInfo2{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,

		.waitSemaphoreInfoCount = 1,
		.pWaitSemaphoreInfos = &waitSemaphoreInfo,

		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &cmdBufInfo,

		.signalSemaphoreInfoCount = 1,
		.pSignalSemaphoreInfos = &signalSemaphoreInfo
	};
	VK_CHECK_THROW(
		vkQueueSubmit2(Context.GetGraphicsQueue(), 1, &submitInfo2, frameDatas_[frameIndex].inFlightFence),
		"failed to submit draw command buffer!"
	);

	const VkPresentInfoKHR presentInfo{
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,

		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &frameDatas_[frameIndex].renderFinishedSemaphore,

		.swapchainCount = 1,
		.pSwapchains = &swapChain_,
		.pImageIndices = &frameDatas_[frameIndex].imageIndex,
	};
	const VkResult result{ vkQueuePresentKHR(Context.GetPresentQueue(), &presentInfo) };

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		RecreateSwapChain();
	}
	else VK_CHECK_THROW(
		result,
		"failed to present swap chain image!"
	);

	if constexpr (IS_MULTI_THREADED)
	{
		if (!TryAcquireNextImage(frameIndex))
		{
			KT_LOG(ELogImportanceLevel::High, "Graphics", "frame {0} skipped", frameCount_);
			return;
		}
	}
}


bool GRenderer::TryAcquireNextImage(const u32 frameIndex)
{
	// Wait for current frame to be rendered
	VK_CHECK_THROW(
		vkWaitForFences(Context.GetDevice(), 1, &frameDatas_[frameIndex].inFlightFence, VK_TRUE, UINT64_MAX),
		"failed to wait for fences!"
	);

	// Set image index for current frame
	const VkResult result{ vkAcquireNextImageKHR(Context.GetDevice(), swapChain_, UINT64_MAX, frameDatas_[frameIndex].imageAvailableSemaphore, VK_NULL_HANDLE, &frameDatas_[frameIndex].imageIndex) };
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return false;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(Context.GetDevice(), 1, &frameDatas_[frameIndex].inFlightFence);

	return true;
}

void GRenderer::JoinThread(std::thread& thread) const
{
	if (thread.joinable())
	{
		thread.join();
	}
}

u32 GRenderer::GetGameThreadFrame() const
{
	// Prepare game thread for render thread
	return frameCount_ % static_cast<u32>(KT_FRAMES_IN_FLIGHT);
}

u32 GRenderer::GetRenderThreadFrame() const
{
	// Prepare render thread for RHI thread
	return ((frameCount_ + KT_FRAMES_IN_FLIGHT) - 1) % static_cast<u32>(KT_FRAMES_IN_FLIGHT); // avoid negative with + KT_FRAMES_IN_FLIGHT
}

u32 GRenderer::GetRHIThreadFrame() const
{
	// Prepare RHI thread for game thread
	return ((frameCount_ + KT_FRAMES_IN_FLIGHT) - 2) % static_cast<u32>(KT_FRAMES_IN_FLIGHT); // avoid negative with + KT_FRAMES_IN_FLIGHT
}

void GRenderer::RecreateSwapChain()
{
	// Wait for CPU
	JoinThread(renderThread_);
	JoinThread(rhiThread_);

	// Wait for GPU
	vkDeviceWaitIdle(Context.GetDevice());

	CleanupSwapChain();

	CreateSwapChain();
	CreateImageViews();
	CreateColorResources();
	CreateDepthResources();

	for (auto& frameData : frameDatas_)
	{
		vkResetCommandPool(Context.GetDevice(), frameData.commandPool, 0);
	}
}

VkExtent2D GRenderer::GetSwapChainExtent() const
{
	return swapChainExtent_;
}

VkFormat GRenderer::GetSwapChainFormat() const
{
	return swapChainFormat_;
}

VkFormat GRenderer::GetDepthFormat() const
{
	return depthFormat_;
}

void GRenderer::CleanupSwapChain()
{
	for (auto& swapChainData : swapChainDatas_)
	{
		vkDestroyImageView(Context.GetDevice(), swapChainData.imageView, nullptr);
	}
	swapChainDatas_.clear();

	for (auto& frameData : frameDatas_)
	{
		vkDestroyImageView(Context.GetDevice(), frameData.colorTarget.imageView, nullptr);
		vmaDestroyImage(Context.GetAllocator(), frameData.colorTarget.image, frameData.colorTarget.allocation);

		vkDestroyImageView(Context.GetDevice(), frameData.depthTarget.imageView, nullptr);
		vmaDestroyImage(Context.GetAllocator(), frameData.depthTarget.image, frameData.depthTarget.allocation);
	}

	vkDestroySwapchainKHR(Context.GetDevice(), swapChain_, nullptr);
	swapChain_ = VK_NULL_HANDLE;
}
