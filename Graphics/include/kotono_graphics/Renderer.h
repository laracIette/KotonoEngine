#pragma once
#include "frames_in_flight.h"
#include <kotono_common/Pool.h>
#include <span>
#include <thread>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
struct UDrawCall;
class GRenderer final
{
	friend class SCore;

private:
	void Init();
	void Cleanup();

public:
	void DrawFrame();

	VkExtent2D GetSwapChainExtent() const;
	VkFormat GetSwapChainFormat() const;
	VkFormat GetDepthFormat() const;

	void RegisterDrawCall(UDrawCall* drawCall);
	void UnregisterDrawCall(UDrawCall* drawCall);

private:
	struct SwapChainData
	{
		VkImage image;
		VkImageView imageView;
	};

	struct AllocatedImage
	{
		VkImage image;
		VmaAllocation allocation;
		VkImageView imageView;
	};

	struct FrameData
	{
		VkCommandPool commandPool;
		VkCommandBuffer commandBuffer;
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		AllocatedImage colorTarget;
		AllocatedImage depthTarget;
		VkFence inFlightFence;
		u32 imageIndex;
	};

private:
	void CreateSwapChain();
	void CleanupSwapChain();
	void RecreateSwapChain();
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::span<VkSurfaceFormatKHR> availableFormats) const;
	VkPresentModeKHR ChooseSwapPresentMode(const std::span<VkPresentModeKHR> availablePresentModes) const;
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	void CreateImageViews();

	void CreateColorResources();
	void CreateDepthResources();
	VkFormat FindSupportedFormat(const std::span<VkFormat> candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) const;
	VkFormat FindDepthFormat() const;

	bool TryAcquireNextImage(const u32 frameIndex);

	void CreateCommandPools();
	void CreateCommandPool(const u32 frameIndex);
	void CreateCommandBuffers();
	void CreateCommandBuffer(const u32 frameIndex);
	void RecordCommandBuffer(const u32 frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer);
	void CmdAcquireBarrier(VkCommandBuffer commandBuffer, const u32 frameIndex);
	void CmdBeginRendering(VkCommandBuffer commandBuffer, const u32 frameIndex);
	void CmdDrawFrame(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdEndRendering(VkCommandBuffer commandBuffer);
	void CmdPresentationBarrier(VkCommandBuffer commandBuffer, const u32 frameIndex);
	void EndCommandBuffer(VkCommandBuffer commandBuffer);
	void SubmitCommandBuffer(const u32 frameIndex);

	void CreateSyncObjects();

	void JoinThread(std::thread& thread) const;

	u32 GetGameThreadFrame() const;
	u32 GetRenderThreadFrame() const;
	u32 GetRHIThreadFrame() const;

private:
	std::vector<SwapChainData> swapChainDatas_;
	VkSwapchainKHR swapChain_;
	VkFormat swapChainFormat_;
	VkExtent2D swapChainExtent_;

	VkFormat depthFormat_;

	KtFramesInFlightArray<FrameData> frameDatas_;

	std::thread renderThread_;
	std::thread rhiThread_;

	u32 frameCount_;

	UPool<UDrawCall*> drawCalls_;
};

inline GRenderer Renderer;
