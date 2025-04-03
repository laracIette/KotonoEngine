#pragma once
#include "ImageTexture.h"
#include "frames_in_flight.h"
#include "ObjectData2D.h"
#include "ObjectData3D.h"
#include "Model.h"
#include <vector>
#include <array>
#include <span>
#include <unordered_map>
#include "Renderer2D.h"
#include "Renderer3D.h"
#include <thread>

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

	std::thread _renderThread;
	std::thread _rhiThread;

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
	void CleanupSwapChain();
	void RecreateSwapChain();
	const VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::span<VkSurfaceFormatKHR> availableFormats) const;
	const VkPresentModeKHR ChooseSwapPresentMode(const std::span<VkPresentModeKHR> availablePresentModes) const;
	const VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	void CreateImageViews();

	void CreateRenderPass();
	void CreateFramebuffers();

	void CreateColorResources();
	void CreateDepthResources();
	const VkFormat FindSupportedFormat(const std::span<VkFormat> candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) const;
	const VkFormat FindDepthFormat() const;
	const bool HasStencilComponent(const VkFormat format) const;

	void CreateCommandBuffers();
	void RecordCommandBuffer(const uint32_t imageIndex, const uint32_t currentFrame) const;
	void SubmitCommandBuffer(const uint32_t imageIndex, const uint32_t currentFrame);

	void CreateSyncObjects();

	void CmdDrawRenderers(VkCommandBuffer commandBuffer) const;
	void ResetRenderers();

	void JoinRHIThread();
	void JoinRenderThread();

	const uint32_t GetRenderThreadFrame(const uint32_t currentFrame) const;
	const uint32_t GetRHIThreadFrame(const uint32_t currentFrame) const;
};