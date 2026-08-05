#pragma once
#include "frames_in_flight.h"
#include "FrameContextBuffer.h"
#include "GPUBuffers.h"
#include "LightBuffers.h"
#include <kotono_common/Pool.h>
#include <kotono_platform/AllocatedImage.h>
#include <span>
#include <thread>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
struct UDrawCall;
class URenderer final
{
public:
	struct FrameData
	{
		VkCommandPool	commandPool;
		VkCommandBuffer commandBuffer;

		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;

		VkFence inFlightFence;
		u32		imageIndex;

		UAllocatedImage colorTarget;
		UAllocatedImage albedoTarget;
		UAllocatedImage normalTarget;
		UAllocatedImage ormTarget;
		UAllocatedImage depthTarget;
	};

public:
	void Init();
	void Cleanup();

	void DrawFrame(const UFrameContextSceneView& sceneView);

	void RegisterOpaqueDrawCall(UDrawCall* drawCall);
	void RegisterInterfaceDrawCall(UDrawCall* drawCall);
	void UnregisterOpaqueDrawCall(UDrawCall* drawCall);
	void UnregisterInterfaceDrawCall(UDrawCall* drawCall);

	void RegisterDirectionalLight(const ULightBuffers::DirectionalLightData& directionalLight);
	void RegisterPointLight(const UPointLight& pointLight);

private:
	void RecreateFrame();

	void CreateImageResources();
	void CleanupImageResources() const;

	void RegisterFrameContextBufferTextures();
	void UnregisterFrameContextBufferTextures() const;

	void CreateSampler();

	bool TryAcquireNextImage(const u32 frameIndex);

	void CreateCommandPools();
	void CreateCommandPool(const u32 frameIndex);
	void CreateCommandBuffers();
	void CreateCommandBuffer(const u32 frameIndex);
	void RecordCommandBuffer(const u32 frameIndex);
	void BeginCommandBuffer(VkCommandBuffer commandBuffer);

	void CmdUseWindowViewport(VkCommandBuffer commandBuffer) const;

	void CmdUpdateClusterAABB(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	
	void CmdBarrierComputeFragmentReadToClearWrite(VkCommandBuffer commandBuffer) const;
	void CmdResetLightCounter(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBarrierComputeClearWriteToReadWrite(VkCommandBuffer commandBuffer) const;
	void CmdDispatchLightBinning(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdBarrierComputeWriteToFragmentRead(VkCommandBuffer commandBuffer) const;

	void CmdDrawFrameShadowMaps(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	
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
	
	void CmdBeginRenderingInterface(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	void CmdDrawFrameInterface(VkCommandBuffer commandBuffer, const u32 frameIndex) const;
	
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
	UFramesInFlightArray<FrameData> frameDatas_;
	u32 samplerIndex_;

	std::thread renderThread_;
	std::thread rhiThread_;

	u32 frameCount_;

	UPool<UDrawCall*> opaqueDrawCalls_;
	UPool<UDrawCall*> interfaceDrawCalls_;

	UFrameContextBuffer frameContextBuffer_;
	ULightBuffers lightBuffers_;
	UGPUBuffers gpuBuffers_;
};
