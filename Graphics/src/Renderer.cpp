#include "Renderer.h"
#include <kotono_platform/Context.h>
#include <kotono_platform/Window.h>
#include <kotono_common/log.h>
#include <kotono_platform/vk_utils.h>

void KtRenderer::Init()
{
	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateColorResources();
	CreateDepthResources();
	CreateFramebuffers();
	CreateCommandPools();
	CreateCommandBuffers();
	CreateSyncObjects();

	interfaceRenderer_.Init();
	sceneRenderer_.Init();
}

void KtRenderer::Cleanup()
{
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "Graphics.KtRenderer::Cleanup()", "cleaning up renderer");

	JoinThread(renderThread_);
	JoinThread(rhiThread_);

	interfaceRenderer_.Cleanup();
	sceneRenderer_.Cleanup();

	CleanupSwapChain();

	for (size_t i{ 0 }; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		vkDestroyCommandPool(Context.GetDevice(), commandPools_[i], nullptr);
	}

	vkDestroyRenderPass(Context.GetDevice(), renderPass_, nullptr);

	for (size_t i{ 0 }; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(Context.GetDevice(), renderFinishedSemaphores_[i], nullptr);
		vkDestroySemaphore(Context.GetDevice(), imageAvailableSemaphores_[i], nullptr);
		vkDestroyFence(Context.GetDevice(), inFlightFences_[i], nullptr);
	}

	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "Graphics.KtRenderer::Cleanup()", "cleaned up renderer");
}

void KtRenderer::CreateSwapChain()
{
	KtSwapChainSupportDetails swapChainSupport = Context.QuerySwapChainSupport(Context.GetPhysicalDevice());

	const VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	const VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	const VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "Graphics.KtRenderer::CreateSwapChain()", "swap chain image count: %u", imageCount);

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = Context.GetSurface();

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const KtQueueFamilyIndices indices = Context.FindQueueFamilies(Context.GetPhysicalDevice());
	const std::array<uint32_t, 2> queueFamilyIndices =
	{
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
		createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	// Used for blending window (here opaque)
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	// Discard pixels that are hidden by other windows
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VK_CHECK_THROW(
		vkCreateSwapchainKHR(Context.GetDevice(), &createInfo, nullptr, &swapChain_),
		"failed to create swap chain!"
	);

	vkGetSwapchainImagesKHR(Context.GetDevice(), swapChain_, &imageCount, nullptr);
	swapChainImages_.resize(imageCount);
	vkGetSwapchainImagesKHR(Context.GetDevice(), swapChain_, &imageCount, swapChainImages_.data());

	swapChainImageFormat_ = surfaceFormat.format;
	swapChainExtent_ = extent;
}

VkSurfaceFormatKHR KtRenderer::ChooseSwapSurfaceFormat(const std::span<VkSurfaceFormatKHR> availableFormats) const
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

VkPresentModeKHR KtRenderer::ChooseSwapPresentMode(const std::span<VkPresentModeKHR> availablePresentModes) const
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

VkExtent2D KtRenderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(Window.GetGLFWWindow(), &width, &height);

		VkExtent2D actualExtent =
		{
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = glm::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = glm::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

void KtRenderer::CreateImageViews()
{
	swapChainImageViews_.resize(swapChainImages_.size());

	for (uint32_t i = 0; i < swapChainImages_.size(); i++)
	{
		swapChainImageViews_[i] = Context.CreateImageView(swapChainImages_[i], swapChainImageFormat_, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}
}

void KtRenderer::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat_;
	colorAttachment.samples = Context.GetMSAASamples();
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = FindDepthFormat();
	depthAttachment.samples = Context.GetMSAASamples();
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription colorAttachmentResolve{};
	colorAttachmentResolve.format = swapChainImageFormat_;
	colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference resolveAttachmentRef{};
	resolveAttachmentRef.attachment = 2;
	resolveAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pResolveAttachments = &resolveAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	const std::array<VkAttachmentDescription, 3> attachments =
	{
		colorAttachment,
		depthAttachment,
		colorAttachmentResolve
	};

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VK_CHECK_THROW(
		vkCreateRenderPass(Context.GetDevice(), &renderPassInfo, nullptr, &renderPass_),
		"failed to create render pass!"
	);
}

void KtRenderer::CreateFramebuffers()
{
	swapChainFramebuffers_.resize(swapChainImageViews_.size());

	for (size_t i = 0; i < swapChainImageViews_.size(); i++)
	{
		const std::array<VkImageView, 3> attachments =
		{
			colorImageView_,
			depthImageView_,
			swapChainImageViews_[i],
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass_;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent_.width;
		framebufferInfo.height = swapChainExtent_.height;
		framebufferInfo.layers = 1;

		VK_CHECK_THROW(
			vkCreateFramebuffer(Context.GetDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers_[i]),
			"failed to create framebuffer!"
		);
	}
}

void KtRenderer::CreateColorResources()
{
	const VkFormat colorFormat = swapChainImageFormat_;

	Context.CreateImage(
		swapChainExtent_.width,
		swapChainExtent_.height,
		1,
		Context.GetMSAASamples(),
		colorFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		colorImage_,
		colorImageAllocation_
	);

	colorImageView_ = Context.CreateImageView(
		colorImage_,
		colorFormat,
		VK_IMAGE_ASPECT_COLOR_BIT,
		1
	);
}

void KtRenderer::CreateDepthResources()
{
	const VkFormat depthFormat = FindDepthFormat();

	Context.CreateImage(
		swapChainExtent_.width,
		swapChainExtent_.height,
		1,
		Context.GetMSAASamples(),
		depthFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		depthImage_,
		depthImageAllocation_
	);

	depthImageView_ = Context.CreateImageView(
		depthImage_,
		depthFormat,
		VK_IMAGE_ASPECT_DEPTH_BIT,
		1
	);

	Context.TransitionImageLayout(
		depthImage_,
		depthFormat,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		1
	);
}

VkFormat KtRenderer::FindSupportedFormat(const std::span<VkFormat> candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) const
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

VkFormat KtRenderer::FindDepthFormat() const
{
	std::vector<VkFormat> formats = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
	return FindSupportedFormat(formats, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool KtRenderer::HasStencilComponent(const VkFormat format) const
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void KtRenderer::CreateCommandPools()
{
	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateCommandPool(static_cast<uint32_t>(i));
	}
}

void KtRenderer::CreateCommandPool(const uint32_t frameIndex)
{
	const KtQueueFamilyIndices queueFamilyIndices = Context.FindQueueFamilies(Context.GetPhysicalDevice());

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	VK_CHECK_THROW(
		vkCreateCommandPool(Context.GetDevice(), &poolInfo, nullptr, &commandPools_[frameIndex]),
		"failed to create command pool!"
	);
}

void KtRenderer::CreateCommandBuffers()
{
	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; ++i)
	{
		CreateCommandBuffer(static_cast<uint32_t>(i));
	}
}

void KtRenderer::CreateCommandBuffer(const uint32_t frameIndex)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPools_[frameIndex];
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Context.GetDevice(), &allocInfo, &commandBuffers_[frameIndex]),
		"failed to allocate command buffers!"
	);
}

void KtRenderer::CreateSyncObjects()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(Context.GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores_[i]) != VK_SUCCESS ||
			vkCreateSemaphore(Context.GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores_[i]) != VK_SUCCESS ||
			vkCreateFence(Context.GetDevice(), &fenceInfo, nullptr, &inFlightFences_[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}
static constexpr bool IS_MULTI_THREADED{ false };
void KtRenderer::DrawFrame()
{
	const uint32_t frameIndex{ GetGameThreadFrame() };
	UpdateRenderers(frameIndex);

	if constexpr (IS_MULTI_THREADED)
	{
		if (frameCount_ >= 1)
		{
			JoinThread(renderThread_);
			const uint32_t renderThreadFrame{ GetRenderThreadFrame() };
			renderThread_ = std::thread(&KtRenderer::RecordCommandBuffer, this, renderThreadFrame);
		}

		if (frameCount_ >= 2)
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "Graphics.KtRenderer::DrawFrame()", "frame %u rendered", frameCount_);

			JoinThread(rhiThread_);
			Context.ExecuteSingleTimeCommands();
			const uint32_t renderRHIFrame{ GetRHIThreadFrame() };
			rhiThread_ = std::thread(&KtRenderer::SubmitCommandBuffer, this, renderRHIFrame);
		}
	}
	else
	{
		if (!TryAcquireNextImage(frameIndex))
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "Graphics.KtRenderer::DrawFrame()", "frame %u skipped", frameCount_);
			return;
		}

		RecordCommandBuffer(frameIndex);
		Context.ExecuteSingleTimeCommands();
		SubmitCommandBuffer(frameIndex);
	}

	frameCount_++;
}

void KtRenderer::UpdateRenderers(const uint32_t frameIndex)
{
	sceneRenderer_.Update(frameIndex);
	interfaceRenderer_.Update(frameIndex);
}

void KtRenderer::RecordCommandBuffer(const uint32_t frameIndex)
{
	VkCommandBuffer commandBuffer = commandBuffers_[frameIndex];
	vkResetCommandBuffer(commandBuffer, 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	VK_CHECK_THROW(
		vkBeginCommandBuffer(commandBuffer, &beginInfo),
		"failed to begin recording command buffer!"
	);

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass_;
	renderPassInfo.framebuffer = GetFramebuffer(frameIndex);
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent_;

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	// Begin RenderPass
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

	CmdDrawRenderers(commandBuffer, frameIndex);

	// End RenderPass
	vkCmdEndRenderPass(commandBuffer);

	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer),
		"failed to record command buffer!"
	);
}

VkFramebuffer& KtRenderer::GetFramebuffer(const uint32_t frameIndex)
{
	return swapChainFramebuffers_[imageIndices_[frameIndex]];
}

VkCommandPool& KtRenderer::GetCommandPool(const uint32_t frameIndex)
{
	return commandPools_[frameIndex];
}

void KtRenderer::CmdDrawRenderers(VkCommandBuffer commandBuffer, const uint32_t frameIndex)
{
	sceneRenderer_.CmdDraw(commandBuffer, frameIndex);
	interfaceRenderer_.CmdDraw(commandBuffer, frameIndex);
}

void KtRenderer::SubmitCommandBuffer(const uint32_t frameIndex)
{
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	const std::array<VkSemaphore, 1> waitSemaphores = { imageAvailableSemaphores_[frameIndex] };
	const std::array<VkPipelineStageFlags, 1> waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
	submitInfo.pWaitSemaphores = waitSemaphores.data();
	submitInfo.pWaitDstStageMask = waitStages.data();

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers_[frameIndex];

	const std::array<VkSemaphore, 1> signalSemaphores = { renderFinishedSemaphores_[frameIndex] };
	submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
	submitInfo.pSignalSemaphores = signalSemaphores.data();

	VK_CHECK_THROW(
		vkQueueSubmit(Context.GetGraphicsQueue(), 1, &submitInfo, inFlightFences_[frameIndex]),
		"failed to submit draw command buffer!"
	);

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
	presentInfo.pWaitSemaphores = signalSemaphores.data();

	const std::array<VkSwapchainKHR, 1> swapChains = { swapChain_ };
	presentInfo.swapchainCount = static_cast<uint32_t>(swapChains.size());
	presentInfo.pSwapchains = swapChains.data();
	presentInfo.pImageIndices = &imageIndices_[frameIndex];
	presentInfo.pResults = nullptr; // Optional

	const VkResult result = vkQueuePresentKHR(Context.GetPresentQueue(), &presentInfo);

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
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "Graphics.KtRenderer::SubmitCommandBuffer()", "frame %u skipped", frameCount_);
			return;
		}
	}
}


bool KtRenderer::TryAcquireNextImage(const uint32_t frameIndex)
{
	// Wait for current frame to be rendered
	vkWaitForFences(Context.GetDevice(), 1, &inFlightFences_[frameIndex], VK_TRUE, UINT64_MAX);

	// Set image index for current frame
	static constexpr uint64_t timeout{ 1000000 };
	const VkResult result = vkAcquireNextImageKHR(Context.GetDevice(), swapChain_, timeout, imageAvailableSemaphores_[frameIndex], VK_NULL_HANDLE, &imageIndices_[frameIndex]);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return false;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(Context.GetDevice(), 1, &inFlightFences_[frameIndex]);

	return true;
}

void KtRenderer::JoinThread(std::thread& thread) const
{
	if (thread.joinable())
	{
		thread.join();
	}
}

uint32_t KtRenderer::GetGameThreadFrame() const
{
	// Prepare game thread for render thread
	return frameCount_ % static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT);
}

uint32_t KtRenderer::GetRenderThreadFrame() const
{
	// Prepare render thread for RHI thread
	return ((frameCount_ + KT_FRAMES_IN_FLIGHT) - 1) % static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT); // avoid negative with + KT_FRAMES_IN_FLIGHT
}

uint32_t KtRenderer::GetRHIThreadFrame() const
{
	// Prepare RHI thread for game thread
	return ((frameCount_ + KT_FRAMES_IN_FLIGHT) - 2) % static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT); // avoid negative with + KT_FRAMES_IN_FLIGHT
}

void KtRenderer::RecreateSwapChain()
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
	CreateFramebuffers();
}

VkExtent2D KtRenderer::GetSwapChainExtent() const
{
	return swapChainExtent_;
}

KtInterfaceRenderer& KtRenderer::GetInterfaceRenderer()
{
	return interfaceRenderer_;
}

KtSceneRenderer& KtRenderer::GetSceneRenderer()
{
	return sceneRenderer_;
}

VkRenderPass& KtRenderer::GetRenderPass()
{
	return renderPass_;
}

void KtRenderer::CleanupSwapChain()
{
	for (auto framebuffer : swapChainFramebuffers_)
	{
		vkDestroyFramebuffer(Context.GetDevice(), framebuffer, nullptr);
	}
	swapChainFramebuffers_.clear();

	for (auto imageView : swapChainImageViews_)
	{
		vkDestroyImageView(Context.GetDevice(), imageView, nullptr);
	}
	swapChainImageViews_.clear();

	vkDestroyImageView(Context.GetDevice(), colorImageView_, nullptr);
	vmaDestroyImage(Context.GetAllocator(), colorImage_, colorImageAllocation_);

	vkDestroyImageView(Context.GetDevice(), depthImageView_, nullptr);
	vmaDestroyImage(Context.GetAllocator(), depthImage_, depthImageAllocation_);

	vkDestroySwapchainKHR(Context.GetDevice(), swapChain_, nullptr);
	swapChain_ = VK_NULL_HANDLE;
	swapChainImages_.clear();
}
