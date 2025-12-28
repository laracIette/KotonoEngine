#pragma once
#include "frames_in_flight.h"
#include "InterfaceRenderer.h"
#include "SceneRenderer.h"
#include <span>
#include <thread>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

class KtRenderer final
{
	friend class SCore;

private:
	void Init();
	void Cleanup();

public:
	void DrawFrame();

	uint32_t GetGameThreadFrame() const; // todo: make private

	VkExtent2D SwapChainExtent() const;

	KtInterfaceRenderer& InterfaceRenderer();
	KtSceneRenderer& SceneRenderer();

	VkRenderPass& RenderPass();
	VkFramebuffer& GetFramebuffer(const uint32_t frameIndex);
	VkCommandPool& GetCommandPool(const uint32_t frameIndex);

private:
	struct SwapChainData
	{
		VkImage image;
		VkImageView imageView;
		VkFramebuffer framebuffer;
	};

	struct FrameData
	{
		VkCommandPool commandPool;
		VkCommandBuffer commandBuffer;
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkFence inFlightFence;
		uint32_t imageIndex;
	};

	KtInterfaceRenderer interfaceRenderer_;
	KtSceneRenderer sceneRenderer_;

	std::vector<SwapChainData> swapChainDatas_;
	VkSwapchainKHR swapChain_;
	VkFormat swapChainImageFormat_;
	VkExtent2D swapChainExtent_;

	VkRenderPass renderPass_;

	KtFramesInFlightArray<FrameData> frameDatas_;

	std::thread renderThread_;
	std::thread rhiThread_;

	VkImage colorImage_;
	VmaAllocation colorImageAllocation_;
	VkImageView colorImageView_;

	VkImage depthImage_;
	VmaAllocation depthImageAllocation_;
	VkImageView depthImageView_;

	uint32_t frameCount_;

	void CreateSwapChain();
	void CleanupSwapChain();
	void RecreateSwapChain();
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::span<VkSurfaceFormatKHR> availableFormats) const;
	VkPresentModeKHR ChooseSwapPresentMode(const std::span<VkPresentModeKHR> availablePresentModes) const;
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	void CreateImageViews();

	void CreateRenderPass();
	void CreateFramebuffers();

	void CreateColorResources();
	void CreateDepthResources();
	VkFormat FindSupportedFormat(const std::span<VkFormat> candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) const;
	VkFormat FindDepthFormat() const;
	bool HasStencilComponent(const VkFormat format) const;

	bool TryAcquireNextImage(const uint32_t frameIndex);

	void CreateCommandPools();
	void CreateCommandPool(const uint32_t frameIndex);
	void CreateCommandBuffers();
	void CreateCommandBuffer(const uint32_t frameIndex);
	void RecordCommandBuffer(const uint32_t frameIndex);
	void SubmitCommandBuffer(const uint32_t frameIndex);

	void CreateSyncObjects();

	void UpdateRenderers(const uint32_t frameIndex);
	void CmdDrawRenderers(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

	void JoinThread(std::thread& thread) const;

	uint32_t GetRenderThreadFrame() const;
	uint32_t GetRHIThreadFrame() const;
};

inline KtRenderer Renderer;
