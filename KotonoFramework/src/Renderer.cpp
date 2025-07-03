#include "Renderer.h"
#include "Framework.h"
#include "Context.h"
#include "Window.h"
#include "log.h"
#include "Viewport.h"
#include "vk_utils.h"
#include "time_utils.h"

void KtRenderer::Init()
{
	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateColorResources();
	CreateDepthResources();
	CreateFramebuffers();
	CreateCommandBuffers();
	CreateSyncObjects();

	renderer2D_.Init();
	renderer3D_.Init();
}

void KtRenderer::Cleanup()
{
	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "cleaning up renderer");

	JoinThread(renderThread_);
	JoinThread(rhiThread_);

	renderer2D_.Cleanup();
	renderer3D_.Cleanup();

	CleanupSwapChain();

	vkDestroyRenderPass(Framework.GetContext().GetDevice(), renderPass_, nullptr);

	for (size_t i = 0; i < KT_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(Framework.GetContext().GetDevice(), renderFinishedSemaphores_[i], nullptr);
		vkDestroySemaphore(Framework.GetContext().GetDevice(), imageAvailableSemaphores_[i], nullptr);
		vkDestroyFence(Framework.GetContext().GetDevice(), inFlightFences_[i], nullptr);
	}

	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "cleaned up renderer");
}

void KtRenderer::CreateSwapChain()
{
	KtSwapChainSupportDetails swapChainSupport = Framework.GetContext().QuerySwapChainSupport(Framework.GetContext().GetPhysicalDevice());

	const VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
	const VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes);
	const VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities);

	uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;
	if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.Capabilities.maxImageCount;
	}
	
	KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "KtRenderer::CreateSwapChain(): swap chain image count: %u", imageCount);

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = Framework.GetContext().GetSurface();

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const KtQueueFamilyIndices indices = Framework.GetContext().FindQueueFamilies(Framework.GetContext().GetPhysicalDevice());
	const std::array<uint32_t, 2> queueFamilyIndices = 
	{ 
		indices.GraphicsFamily.value(), 
		indices.PresentFamily.value() 
	};

	if (indices.GraphicsFamily != indices.PresentFamily)
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

	createInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
	// Used for blending window (here opaque)
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	// Discard pixels that are hidden by other windows
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VK_CHECK_THROW(
		vkCreateSwapchainKHR(Framework.GetContext().GetDevice(), &createInfo, nullptr, &swapChain_),
		"failed to create swap chain!"
	);

	vkGetSwapchainImagesKHR(Framework.GetContext().GetDevice(), swapChain_, &imageCount, nullptr);
	swapChainImages_.resize(imageCount);
	vkGetSwapchainImagesKHR(Framework.GetContext().GetDevice(), swapChain_, &imageCount, swapChainImages_.data());

	swapChainImageFormat_ = surfaceFormat.format;
	swapChainExtent_ = extent;
}

const VkSurfaceFormatKHR KtRenderer::ChooseSwapSurfaceFormat(const std::span<VkSurfaceFormatKHR> availableFormats) const
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

const VkPresentModeKHR KtRenderer::ChooseSwapPresentMode(const std::span<VkPresentModeKHR> availablePresentModes) const
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

const VkExtent2D KtRenderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(Framework.GetWindow().GetGLFWWindow(), &width, &height);

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
		swapChainImageViews_[i] = Framework.GetContext().CreateImageView(swapChainImages_[i], swapChainImageFormat_, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}
}

void KtRenderer::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat_;
	colorAttachment.samples = Framework.GetContext().GetMSAASamples();
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = FindDepthFormat();
	depthAttachment.samples = Framework.GetContext().GetMSAASamples();
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
		vkCreateRenderPass(Framework.GetContext().GetDevice(), &renderPassInfo, nullptr, &renderPass_),
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
			vkCreateFramebuffer(Framework.GetContext().GetDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers_[i]),
			"failed to create framebuffer!"
		);
	}
}

void KtRenderer::CreateColorResources()
{
	const VkFormat colorFormat = swapChainImageFormat_;

	Framework.GetContext().CreateImage(
		swapChainExtent_.width,
		swapChainExtent_.height,
		1,
		Framework.GetContext().GetMSAASamples(),
		colorFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		colorImage_,
		colorImageAllocation_
	);

	colorImageView_ = Framework.GetContext().CreateImageView(
		colorImage_, 
		colorFormat, 
		VK_IMAGE_ASPECT_COLOR_BIT,
		1
	);
}

void KtRenderer::CreateDepthResources()
{
	const VkFormat depthFormat = FindDepthFormat();

	Framework.GetContext().CreateImage(
		swapChainExtent_.width,
		swapChainExtent_.height,
		1,
		Framework.GetContext().GetMSAASamples(),
		depthFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		depthImage_,
		depthImageAllocation_
	);

	depthImageView_ = Framework.GetContext().CreateImageView(
		depthImage_, 
		depthFormat, 
		VK_IMAGE_ASPECT_DEPTH_BIT, 
		1
	);

	Framework.GetContext().TransitionImageLayout(
		depthImage_,
		depthFormat,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		1
	);
}

const VkFormat KtRenderer::FindSupportedFormat(const std::span<VkFormat> candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) const
{
	for (const VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(Framework.GetContext().GetPhysicalDevice(), format, &props);

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

const VkFormat KtRenderer::FindDepthFormat() const
{
	std::vector<VkFormat> formats = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
	return FindSupportedFormat(formats, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

const bool KtRenderer::HasStencilComponent(const VkFormat format) const
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void KtRenderer::CreateCommandBuffers()
{
	commandBuffers_.resize(KT_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = Framework.GetContext().GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers_.size();

	VK_CHECK_THROW(
		vkAllocateCommandBuffers(Framework.GetContext().GetDevice(), &allocInfo, commandBuffers_.data()),
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
		if (vkCreateSemaphore(Framework.GetContext().GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores_[i]) != VK_SUCCESS ||
			vkCreateSemaphore(Framework.GetContext().GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores_[i]) != VK_SUCCESS ||
			vkCreateFence(Framework.GetContext().GetDevice(), &fenceInfo, nullptr, &inFlightFences_[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}
constexpr bool MULTI_THREADED = false;
void KtRenderer::DrawFrame()
{
	const uint32_t currentFrame = GetGameThreadFrame(); // todo: replace by enum
														// getframe(gamethread), 
														// has to propagate to renderer 2d and 3d
	
#if MULTI_THREADED
	if (frameCount_ >= 1)
	{
		JoinThread(renderThread_);
		const uint32_t renderThreadFrame = GetRenderThreadFrame(currentFrame);
		renderThread _= std::thread(&KtRenderer::RecordCommandBuffer, this, renderThreadFrame);
	}

	if (frameCount_ >= 2)
	{
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "frame %u rendered", frameCount_);

		JoinThread(rhiThread_);
		const uint32_t renderRHIFrame = GetRHIThreadFrame(currentFrame);
		rhiThread _= std::thread(&KtRenderer::SubmitCommandBuffer, this, renderRHIFrame);
	}
#else
	if (!TryAcquireNextImage(currentFrame))
	{
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "KtRenderer::DrawFrame(): frame skipped");
		return;
	}
	RecordCommandBuffer(currentFrame);
	SubmitCommandBuffer(currentFrame);
#endif
	
	frameCount_++;

	ResetRenderers(GetGameThreadFrame());
}

void KtRenderer::RecordCommandBuffer(const uint32_t currentFrame) 
{
	VkCommandBuffer commandBuffer = commandBuffers_[currentFrame];
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
	renderPassInfo.framebuffer = swapChainFramebuffers_[imageIndices_[currentFrame]];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent_;

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	// Begin RenderPass
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	CmdDrawRenderers(commandBuffer, currentFrame);

	// End RenderPass
	vkCmdEndRenderPass(commandBuffer);

	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer), 
		"failed to record command buffer!"
	);
}

void KtRenderer::CmdDrawRenderers(VkCommandBuffer commandBuffer, const uint32_t currentFrame)
{
	renderer3D_.CmdDraw(commandBuffer, currentFrame);
	renderer2D_.CmdDraw(commandBuffer, currentFrame);
}

void KtRenderer::ResetRenderers(const uint32_t currentFrame)
{
	renderer2D_.Reset(currentFrame);
	renderer3D_.Reset(currentFrame);
}

void KtRenderer::SubmitCommandBuffer(const uint32_t currentFrame)
{
	// TODO: idk
	// todo: figure that shit out
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	const std::array<VkSemaphore, 1> waitSemaphores = { imageAvailableSemaphores_[currentFrame] };
	const std::array<VkPipelineStageFlags, 1> waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
	submitInfo.pWaitSemaphores = waitSemaphores.data();
	submitInfo.pWaitDstStageMask = waitStages.data();

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers_[currentFrame];

	const std::array<VkSemaphore, 1> signalSemaphores = { renderFinishedSemaphores_[currentFrame] };
	submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
	submitInfo.pSignalSemaphores = signalSemaphores.data();

	VK_CHECK_THROW(
		vkQueueSubmit(Framework.GetContext().GetGraphicsQueue(), 1, &submitInfo, inFlightFences_[currentFrame]),
		"failed to submit draw command buffer!"
	);

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
	presentInfo.pWaitSemaphores = signalSemaphores.data();

	const std::array<VkSwapchainKHR, 1> swapChains = { swapChain_ };
	presentInfo.swapchainCount = static_cast<uint32_t>(swapChains.size());
	presentInfo.pSwapchains = swapChains.data();
	presentInfo.pImageIndices = &imageIndices_[currentFrame];
	presentInfo.pResults = nullptr; // Optional

	const VkResult result = vkQueuePresentKHR(Framework.GetContext().GetPresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		RecreateSwapChain();
	}
	else VK_CHECK_THROW(
		result,
		"failed to present swap chain image!"
	);
#if MULTI_THREADED
	if (!TryAcquireNextImage(currentFrame))
	{
		KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "KtRenderer::DrawFrame(): frame skipped");
		return;
	}
#endif
}


const bool KtRenderer::TryAcquireNextImage(const uint32_t currentFrame)
{
	// Wait for current frame to be rendered
	vkWaitForFences(Framework.GetContext().GetDevice(), 1, &inFlightFences_[currentFrame], VK_TRUE, UINT64_MAX);

	// Set image index for current frame
	constexpr uint64_t timeout = ms_to_ns(1llu);
	const VkResult result = vkAcquireNextImageKHR(Framework.GetContext().GetDevice(), swapChain_, timeout, imageAvailableSemaphores_[currentFrame], VK_NULL_HANDLE, &imageIndices_[currentFrame]);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return false;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(Framework.GetContext().GetDevice(), 1, &inFlightFences_[currentFrame]);

	return true;
}

void KtRenderer::JoinThread(std::thread& thread) const
{
	if (thread.joinable())
	{
		thread.join();
	}
}

const uint32_t KtRenderer::GetGameThreadFrame() const
{
	return frameCount_ % static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT);
}

const uint32_t KtRenderer::GetRenderThreadFrame(const uint32_t currentFrame) const
{
	// Prepare render thread for RHI thread
	return ((currentFrame + KT_FRAMES_IN_FLIGHT) - 1) % static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT); // avoid negative
}

const uint32_t KtRenderer::GetRHIThreadFrame(const uint32_t currentFrame) const
{
	// Prepare RHI thread for game thread
	return ((currentFrame + KT_FRAMES_IN_FLIGHT) - 2) % static_cast<uint32_t>(KT_FRAMES_IN_FLIGHT); // avoid negative
}

void KtRenderer::RecreateSwapChain()
{
	// Wait for CPU
	JoinThread(renderThread_);
	JoinThread(rhiThread_);

	// Wait for GPU
	vkDeviceWaitIdle(Framework.GetContext().GetDevice());

	CleanupSwapChain();

	CreateSwapChain();
	CreateImageViews();
	CreateColorResources();
	CreateDepthResources();
	CreateFramebuffers();
}

const VkExtent2D KtRenderer::GetSwapChainExtent() const
{
	return swapChainExtent_;
}

void KtRenderer::AddToRenderQueue2D(const KtAddToRenderQueue2DArgs& args)
{
	renderer2D_.AddToRenderQueue(args);
}

void KtRenderer::AddToRenderQueue3D(KtRenderable3DProxy* proxy)
{
	renderer3D_.Register(proxy);
}

void KtRenderer::RemoveFromRenderQueue3D(KtRenderable3DProxy* proxy)
{
	renderer3D_.Remove(proxy);
}

void KtRenderer::SetUniformData3D(const KtUniformData3D& data)
{
	renderer3D_.SetUniformData(data);
}

VkRenderPass KtRenderer::GetRenderPass() const
{
	return renderPass_;
}

void KtRenderer::CleanupSwapChain()
{
	for (auto framebuffer : swapChainFramebuffers_)
	{
		vkDestroyFramebuffer(Framework.GetContext().GetDevice(), framebuffer, nullptr);
	}
	swapChainFramebuffers_.clear();

	for (auto imageView : swapChainImageViews_)
	{
		vkDestroyImageView(Framework.GetContext().GetDevice(), imageView, nullptr);
	}
	swapChainImageViews_.clear();

	vkDestroyImageView(Framework.GetContext().GetDevice(), colorImageView_, nullptr);
	vmaDestroyImage(Framework.GetContext().GetAllocator(), colorImage_, colorImageAllocation_);
	
	vkDestroyImageView(Framework.GetContext().GetDevice(), depthImageView_, nullptr);
	vmaDestroyImage(Framework.GetContext().GetAllocator(), depthImage_, depthImageAllocation_);

	vkDestroySwapchainKHR(Framework.GetContext().GetDevice(), swapChain_, nullptr);
	swapChain_ = VK_NULL_HANDLE;
	swapChainImages_.clear();
}
