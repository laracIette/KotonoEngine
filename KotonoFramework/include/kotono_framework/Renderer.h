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


	const uint32_t GetGameThreadFrame() const; // todo: make private

	const VkExtent2D GetSwapChainExtent() const;

	KtRenderer2D& GetRenderer2D();
	KtRenderer3D& GetRenderer3D();

	VkRenderPass GetRenderPass() const;
	VkFramebuffer& GetFramebuffer(const uint32_t frameIndex);

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

	FramesInFlightArray<VkCommandBuffer> commandBuffers_;

	std::thread renderThread_;
	std::thread rhiThread_;
	std::mutex renderMutex_;

	VkImage colorImage_;
	VmaAllocation colorImageAllocation_;
	VkImageView colorImageView_;

	VkImage depthImage_;
	VmaAllocation depthImageAllocation_;
	VkImageView depthImageView_;

	FramesInFlightArray<VkSemaphore> imageAvailableSemaphores_;
	FramesInFlightArray<VkSemaphore> renderFinishedSemaphores_;
	FramesInFlightArray<VkFence> inFlightFences_;
	FramesInFlightArray<uint32_t> imageIndices_;

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

	const bool TryAcquireNextImage(const uint32_t frameIndex);

	void CreateCommandBuffers();
	void RecordCommandBuffer(const uint32_t frameIndex);
	void SubmitCommandBuffer(const uint32_t frameIndex);

	void CreateSyncObjects();

	void UpdateRenderers(const uint32_t frameIndex);
	void CmdDrawRenderers(VkCommandBuffer commandBuffer, const uint32_t frameIndex);

	void JoinThread(std::thread& thread) const;

	const uint32_t GetRenderThreadFrame() const;
	const uint32_t GetRHIThreadFrame() const;
};