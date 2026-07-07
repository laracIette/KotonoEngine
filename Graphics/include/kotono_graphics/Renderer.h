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
	friend class GCore;

public:
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
		AllocatedImage albedoTarget;
		AllocatedImage normalTarget;
		AllocatedImage ormTarget;
		AllocatedImage depthTarget;
		VkFence inFlightFence;
		u32 imageIndex;
	};

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

	VkImageView GetGBufferAlbedoImageView(const u32 frameIndex) const;
	VkImageView GetGBufferNormalImageView(const u32 frameIndex) const;
	VkImageView GetGBufferORMImageView(const u32 frameIndex) const;
	VkImageView GetGBufferDepthImageView(const u32 frameIndex) const;
	VkImageView GetColorTargetImageView(const u32 frameIndex) const;

private:
	void CreateSwapChain();
	void CleanupSwapChain();
	void RecreateSwapChain();
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::span<VkSurfaceFormatKHR> availableFormats) const;
	VkPresentModeKHR ChooseSwapPresentMode(const std::span<VkPresentModeKHR> availablePresentModes) const;
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	void CreateSwapChainImageViews();

	void CreateImageResources();
	VkFormat FindSupportedFormat(const std::span<VkFormat> candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) const;
	VkFormat FindDepthFormat() const;

	bool TryAcquireNextImage(const u32 frameIndex);

	void CreateCommandPools();
	void CreateCommandPool(const u32 frameIndex);
	void CreateCommandBuffers();
	void CreateCommandBuffer(const u32 frameIndex);
	void RecordCommandBuffer(const u32 frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer);

	void CmdUpdateClusterAABB(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	
	void CmdBarrierComputeFragmentReadToClearWrite(VkCommandBuffer commandBuffer) const;
	void CmdResetLightCounter(VkCommandBuffer commandBuffer) const;
	void CmdBarrierComputeClearWriteToReadWrite(VkCommandBuffer commandBuffer) const;
	void CmdDispatchLightBinning(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBarrierComputeWriteToFragmentRead(VkCommandBuffer commandBuffer) const;
	
	
	void CmdBarrierDepthNoneToWrite(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBeginRenderingDepthPrePass(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdDrawFrameDepthPrePass(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBarrierDepthWriteToRead(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	
	void CmdBarrierGBufferNoneToWrite(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBeginRenderingGBuffer(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdDrawFrameGBuffer(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBarrierGBufferWriteToRead(VkCommandBuffer commandBuffer, const u32 frameIndex) const;

	void CmdBarrierDepthReadToShaderRead(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBarrierColorNoneToWrite(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBeginRenderingDeferredLighting(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdDrawFrameDeferredLighting(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBarrierColorWriteToRead(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	
	void CmdBarrierSwapchainNoneToWrite(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBeginRenderingPostProcess(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdDrawFramePostProcess(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBarrierSwapchainWriteToPresent(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	
	void CmdPushConstants(VkCommandBuffer commandBuffer, const UDrawCall* drawCall, const u32 frameIndex) const;
	void CmdDraw(VkCommandBuffer commandBuffer, const UDrawCall* drawCall) const;
	
	void CmdEndRendering(VkCommandBuffer commandBuffer) const;
	
	void EndCommandBuffer(VkCommandBuffer commandBuffer) const;
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

	UFramesInFlightArray<FrameData> frameDatas_;

	std::thread renderThread_;
	std::thread rhiThread_;

	u32 frameCount_;

	UPool<UDrawCall*> drawCalls_;
};

inline GRenderer Renderer;
