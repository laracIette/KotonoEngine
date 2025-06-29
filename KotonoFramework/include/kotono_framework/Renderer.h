#pragma once
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
#include "frames_in_flight.h"
#include <vector>
#include <array>
#include <span>
#include "Renderer2D.h"
#include "Renderer3D.h"
#include <thread>
#include <mutex>

class KtRenderer final
{
public:
	void Init();
	void Cleanup();

	void DrawFrame();

	const uint32_t GetGameThreadFrame() const;

	const VkExtent2D GetSwapChainExtent() const;

	void AddToRenderQueue2D(const KtAddToRenderQueue2DArgs& args);
	void AddToRenderQueue3D(const KtAddToRenderQueue3DArgs& args);
	void SetUniformData3D(const KtUniformData3D& data);

	VkRenderPass GetRenderPass() const;

private:
	KtRenderer2D renderer2D_;
	KtRenderer3D renderer3D_;

	VkSwapchainKHR swapChain_;
	std::vector<VkImage> swapChainImages_;
	VkFormat swapChainImageFormat_;
	VkExtent2D swapChainExtent_;
	std::vector<VkImageView> swapChainImageViews_;
	std::vector<VkFramebuffer> swapChainFramebuffers_;

	VkRenderPass renderPass_;

	std::vector<VkCommandBuffer> commandBuffers_;

	std::thread renderThread_;
	std::thread rhiThread_;
	std::mutex renderMutex_;

	VkImage colorImage_;
	VmaAllocation colorImageAllocation_;
	VkImageView colorImageView_;

	VkImage depthImage_;
	VmaAllocation depthImageAllocation_;
	VkImageView depthImageView_;

	std::array<VkSemaphore, KT_FRAMES_IN_FLIGHT> imageAvailableSemaphores_;
	std::array<VkSemaphore, KT_FRAMES_IN_FLIGHT> renderFinishedSemaphores_;
	std::array<VkFence, KT_FRAMES_IN_FLIGHT> inFlightFences_;
	std::array<uint32_t, KT_FRAMES_IN_FLIGHT> imageIndices_;

	uint32_t frameCount_;

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

	const bool TryAcquireNextImage(const uint32_t currentFrame);

	void CreateCommandBuffers();
	void RecordCommandBuffer(const uint32_t currentFrame);
	void SubmitCommandBuffer(const uint32_t currentFrame);

	void CreateSyncObjects();

	void CmdDrawRenderers(VkCommandBuffer commandBuffer, const uint32_t currentFrame) const;
	void ResetRenderers(const uint32_t currentFrame);

	void JoinThread(std::thread& thread) const;

	const uint32_t GetRenderThreadFrame(const uint32_t currentFrame) const;
	const uint32_t GetRHIThreadFrame(const uint32_t currentFrame) const;
};