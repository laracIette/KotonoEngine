#pragma once
#include "ImageTexture.h"
#include "frames_in_flight.h"
#include "ObjectData2D.h"
#include "ObjectData3D.h"
#include "Shader2D.h"
#include "Shader3D.h"
#include "Model.h"
#include <vector>
#include <array>
#include <unordered_map>
#include "Renderer2D.h"
#include "Renderer3D.h"

class KtRenderer final
{
public:
	void Init();
	void Cleanup();

	void DrawFrame();

	void OnFramebufferResized();

	const VkExtent2D GetSwapChainExtent() const;

	KtRenderer2D& GetRenderer2D();
	KtRenderer3D& GetRenderer3D();

	VkRenderPass GetRenderPass() const;

private:
	void CreateShaderAndModels() const;

	KtRenderer2D _renderer2D;
	KtRenderer3D _renderer3D;

	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;
	std::vector<VkImageView> _swapChainImageViews;
	std::vector<VkFramebuffer> _swapChainFramebuffers;

	VkRenderPass _renderPass;

	std::vector<VkCommandBuffer> _commandBuffers;

	VkImage _colorImage;
	VmaAllocation _colorImageAllocation;
	VkImageView _colorImageView;

	VkImage _depthImage;
	VmaAllocation _depthImageAllocation;
	VkImageView _depthImageView;

	std::array<VkSemaphore, KT_FRAMES_IN_FLIGHT> _imageAvailableSemaphores;
	std::array<VkSemaphore, KT_FRAMES_IN_FLIGHT> _renderFinishedSemaphores;
	std::array<VkFence, KT_FRAMES_IN_FLIGHT> _inFlightFences;

	bool _framebufferResized;

	uint32_t _frameCount;
	uint32_t _currentFrame;

	void CreateSwapChain();
	void RecreateSwapChain();
	const VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
	const VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
	const VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	void CreateImageViews();

	void CreateRenderPass();
	void CreateFramebuffers();

	void CreateColorResources();
	void CreateDepthResources();
	const VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
	const VkFormat FindDepthFormat() const;
	const bool HasStencilComponent(VkFormat format) const;

	void CreateCommandBuffers();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, const uint32_t imageIndex) const;

	void CreateSyncObjects();

	void ClearRenderQueues();

	void CleanupSwapChain();
};