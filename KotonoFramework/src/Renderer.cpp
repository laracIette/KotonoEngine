#include "Renderer.h"
#include "Framework.h"
#include <chrono>
#include "Mesh.h"
#include "log.h"
#include "vk_utils.h"

KtShader* shader = nullptr;
KtModel* model1 = nullptr;
KtModel* model2 = nullptr;
KtMesh* mesh1 = nullptr;
KtMesh* mesh2 = nullptr;

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

	CreateShaderAndModels();

	_framebufferResized = false;
	_currentFrame = 0;
}

void KtRenderer::Cleanup()
{
	KT_DEBUG_LOG("cleaning up renderer");

	delete shader;
	delete model1;
	delete model2;
	delete mesh1;
	delete mesh2;

	CleanupSwapChain();

	const auto& device = Framework.GetContext().GetDevice();
	const auto& allocator = Framework.GetContext().GetAllocator();

	vkDestroyImageView(device, _depthImageView, nullptr);
	vmaDestroyImage(allocator, _depthImage, _depthImageAllocation);

	vkDestroyImageView(device, _colorImageView, nullptr);
	vmaDestroyImage(allocator, _colorImage, _colorImageAllocation);

	vkDestroyRenderPass(device, _renderPass, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(device, _renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, _imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, _inFlightFences[i], nullptr);
	}

	KT_DEBUG_LOG("cleaned up renderer");
}

void KtRenderer::AddToRenderQueue3D(KtShader* shader, KtModel* model, const KtObjectData3D& objectData)
{
	_renderQueue3D.Shaders[shader].Models[model].ObjectDatas.push_back(objectData);
}

void KtRenderer::SetUniformData3D(const KtUniformData3D& uniformData3D)
{
	_uniformData3D = uniformData3D;
}

void KtRenderer::CreateShaderAndModels() const
{
	shader = new KtShader(
		R"(C:\Users\nicos\Documents\Visual Studio 2022\Projects\KotonoEngine\KotonoFramework\shaders\vulkanVert.spv)",
		R"(C:\Users\nicos\Documents\Visual Studio 2022\Projects\KotonoEngine\KotonoFramework\shaders\vulkanFrag.spv)"
	);
	
	model1 = new KtModel(R"(C:\Users\nicos\Documents\Visual Studio 2022\Projects\KotonoEngine\assets\models\viking_room.obj)");
	model2 = new KtModel(R"(C:\Users\nicos\OneDrive - e-artsup\B2\Environment\Corridor\SM_Column_low.fbx)");

	mesh1 = new KtMesh();
	mesh1->SetShader(shader);
	mesh1->SetModel(model1);

	mesh2 = new KtMesh();
	mesh2->SetShader(shader);
	mesh2->SetModel(model2);
}

void KtRenderer::CreateSwapChain()
{
	const KtSwapChainSupportDetails swapChainSupport = Framework.GetContext().QuerySwapChainSupport(Framework.GetContext().GetPhysicalDevice());

	const VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
	const VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes);
	const VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities);

	uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;
	if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.Capabilities.maxImageCount;
	}

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
	const uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

	if (indices.GraphicsFamily != indices.PresentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
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

	if (vkCreateSwapchainKHR(Framework.GetContext().GetDevice(), &createInfo, nullptr, &_swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(Framework.GetContext().GetDevice(), _swapChain, &imageCount, nullptr);
	_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(Framework.GetContext().GetDevice(), _swapChain, &imageCount, _swapChainImages.data());

	_swapChainImageFormat = surfaceFormat.format;
	_swapChainExtent = extent;
}

const VkSurfaceFormatKHR KtRenderer::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
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

const VkPresentModeKHR KtRenderer::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const
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
	_swapChainImageViews.resize(_swapChainImages.size());

	for (uint32_t i = 0; i < _swapChainImages.size(); i++)
	{
		_swapChainImageViews[i] = Framework.GetContext().CreateImageView(_swapChainImages[i], _swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}
}

void KtRenderer::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = _swapChainImageFormat;
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
	colorAttachmentResolve.format = _swapChainImageFormat;
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

	std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(Framework.GetContext().GetDevice(), &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create render pass!");
	}
}

void KtRenderer::CreateFramebuffers()
{
	_swapChainFramebuffers.resize(_swapChainImageViews.size());

	for (size_t i = 0; i < _swapChainImageViews.size(); i++)
	{
		std::array<VkImageView, 3> attachments = {
			_colorImageView,
			_depthImageView,
			_swapChainImageViews[i],
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = _swapChainExtent.width;
		framebufferInfo.height = _swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(Framework.GetContext().GetDevice(), &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void KtRenderer::CreateColorResources()
{
	VkFormat colorFormat = _swapChainImageFormat;

	Framework.GetContext().CreateImage(
		_swapChainExtent.width,
		_swapChainExtent.height,
		1,
		Framework.GetContext().GetMSAASamples(),
		colorFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		_colorImage,
		_colorImageAllocation
	);

	_colorImageView = Framework.GetContext().CreateImageView(
		_colorImage, 
		colorFormat, 
		VK_IMAGE_ASPECT_COLOR_BIT,
		1
	);
}

void KtRenderer::CreateDepthResources()
{
	const VkFormat depthFormat = FindDepthFormat();

	Framework.GetContext().CreateImage(
		_swapChainExtent.width,
		_swapChainExtent.height,
		1,
		Framework.GetContext().GetMSAASamples(),
		depthFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		_depthImage,
		_depthImageAllocation
	);

	_depthImageView = Framework.GetContext().CreateImageView(
		_depthImage, 
		depthFormat, 
		VK_IMAGE_ASPECT_DEPTH_BIT, 
		1
	);

	Framework.GetContext().TransitionImageLayout(
		_depthImage,
		depthFormat,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		1
	);
}

const VkFormat KtRenderer::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
{
	for (VkFormat format : candidates)
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
	return FindSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

const bool KtRenderer::HasStencilComponent(VkFormat format) const
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void KtRenderer::CreateCommandBuffers()
{
	_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = Framework.GetContext().GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();

	if (vkAllocateCommandBuffers(Framework.GetContext().GetDevice(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void KtRenderer::RecordCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t imageIndex) const
{
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
	renderPassInfo.renderPass = _renderPass;
	renderPassInfo.framebuffer = _swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = _swapChainExtent;

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	// Begin RenderPass
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(_swapChainExtent.width);
	viewport.height = static_cast<float>(_swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = _swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	for (auto& [shader, shaderData] : _renderQueue3D.Shaders)
	{
		if (!shader)
		{
			KT_DEBUG_LOG("shader is nullptr");
			continue;
		}

		shader->UpdateUniformBuffer(_uniformData3D, _currentFrame);

		std::vector<KtObjectData3D> objectBufferData;
		for (auto& [model, modelData] : shaderData.Models)
		{
			objectBufferData.insert(objectBufferData.end(), modelData.ObjectDatas.begin(), modelData.ObjectDatas.end());
		}
		shader->UpdateObjectBuffer(objectBufferData, _currentFrame);

		shader->CmdBind(commandBuffer);
		shader->CmdBindDescriptorSets(commandBuffer, _currentFrame);

		uint32_t instanceIndex = 0;	

		for (auto& [model, modelData] : shaderData.Models)
		{
			const uint32_t instanceCount = static_cast<uint32_t>(modelData.ObjectDatas.size());
			
			if (!model)
			{
				KT_DEBUG_LOG("model is nullptr");
			}
			else if (instanceCount == 0)
			{
				KT_DEBUG_LOG("objectDatas is empty");
			}
			else
			{
				model->CmdBind(commandBuffer);
				model->CmdDraw(commandBuffer, instanceCount, instanceIndex);
			}

			instanceIndex += instanceCount;

			//KT_DEBUG_LOG("Drawing %s", model->GetPath().string().c_str());
		}
	}

	// End RenderPass
	vkCmdEndRenderPass(commandBuffer);

	VK_CHECK_THROW(
		vkEndCommandBuffer(commandBuffer), 
		"failed to record command buffer!"
	);
}

void KtRenderer::CreateSyncObjects()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(Framework.GetContext().GetDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(Framework.GetContext().GetDevice(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(Framework.GetContext().GetDevice(), &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS)
		{

			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}

void KtRenderer::ClearRenderQueue()
{
	_renderQueue3D = {};
}

void KtRenderer::DrawFrame()
{
	static const auto startTime = std::chrono::high_resolution_clock::now();
	const auto currentTime = std::chrono::high_resolution_clock::now();
	const float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	const float uboTime = time / 10.0f;
	KtUniformData3D ubo{};
	ubo.View = glm::lookAt(glm::vec3(cos(uboTime) * 10.0f, sin(uboTime) * 10.0f, 3.0f), glm::vec3(cos(uboTime) * 5.0f, sin(uboTime) * 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.Projection = glm::perspective(glm::radians(45.0f), _swapChainExtent.width / (float)_swapChainExtent.height, 0.1f, 1000.0f);
	ubo.Projection[1][1] *= -1.0f;
	SetUniformData3D(ubo);

	mesh1->AddToRenderQueue3D(
		glm::scale(
			glm::rotate(
				glm::translate(
					glm::mat4(1.0f),
					glm::vec3(0.0f)
				),
				time * glm::radians(90.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)
			),
			glm::vec3(1.0f)
		)
	);
	for (uint32_t i = 0; i < 1000; i++)
	{
		mesh2->AddToRenderQueue3D(
			glm::scale(
				glm::rotate(
					glm::translate(
						glm::mat4(1.0f),
						glm::vec3(cos(i), sin(i), 0.0f) * 0.01f * static_cast<float>(i)
					),
					time * glm::radians(90.0f),
					glm::vec3(0.0f, 0.0f, 1.0f)
				),
				glm::vec3(0.1f)
			)
		);
	}

	vkWaitForFences(Framework.GetContext().GetDevice(), 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(Framework.GetContext().GetDevice(), _swapChain, UINT64_MAX, _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(Framework.GetContext().GetDevice(), 1, &_inFlightFences[_currentFrame]);

	vkResetCommandBuffer(_commandBuffers[_currentFrame], 0);
	RecordCommandBuffer(_commandBuffers[_currentFrame], imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { _imageAvailableSemaphores[_currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffers[_currentFrame];

	VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(Framework.GetContext().GetGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { _swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(Framework.GetContext().GetPresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized)
	{
		_framebufferResized = false;
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

	ClearRenderQueue();

	_currentFrame = (_currentFrame + 1) % static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
}

void KtRenderer::RecreateSwapChain()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(Framework.GetWindow().GetGLFWWindow(), &width, &height);     // Replace to only freeze render
	while (width == 0 || height == 0)                                                   //
	{                                                                                   //
		glfwGetFramebufferSize(Framework.GetWindow().GetGLFWWindow(), &width, &height); //
		glfwWaitEvents();                                                               //
	}                                                                                   //

	vkDeviceWaitIdle(Framework.GetContext().GetDevice());

	CleanupSwapChain();

	CreateSwapChain();
	CreateImageViews();
	CreateColorResources();
	CreateDepthResources();
	CreateFramebuffers();
}

void KtRenderer::OnFramebufferResized()
{
	_framebufferResized = true;
}

const VkExtent2D KtRenderer::GetSwapChainExtent() const
{
	return _swapChainExtent;
}

VkRenderPass KtRenderer::GetRenderPass() const
{
	return _renderPass;
}

void KtRenderer::CleanupSwapChain() const
{
	for (auto imageView : _swapChainImageViews)
	{
		vkDestroyImageView(Framework.GetContext().GetDevice(), imageView, nullptr);
	}

	for (auto framebuffer : _swapChainFramebuffers)
	{
		vkDestroyFramebuffer(Framework.GetContext().GetDevice(), framebuffer, nullptr);
	}

	vkDestroySwapchainKHR(Framework.GetContext().GetDevice(), _swapChain, nullptr);
}