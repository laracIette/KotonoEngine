#pragma once
#include "DrawDataBuffer.h"
#include "FrameContextBuffer.h"
#include "GPUBuffers.h"
#include "LightBuffers.h"
#include "ParametersBuffer.h"
#include "TransformBuffer.h"
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedImage.h>
#include <span>
#include <vector>
#include <vulkan/vulkan_core.h>
struct UDrawCommand;
struct UFrameContextSceneView;
class USceneRender final
{
public:
	void Init(glm::uvec2 const& extent, VkFormat swapChainFormat);
	void Cleanup() const;

	void SetExtent(glm::uvec2 const& extent, VkFormat swapChainFormat);

	u32 GetRenderTarget() const;
	u32 GetDirectionalLightShadowMapTargetIndex(u32 index) const;

	void UpdateBuffers(UFrameContextSceneView const& sceneView, std::span<UDrawCommand const> drawCommands, std::span<UDirectionalLight const> directionalLights, std::span<UPointLight const> pointLights) const;

	/// Step 1: Depth
	/// - Out: Depth
	/// 
	/// Step 2: GBuffer
	/// - In: Depth
	/// - Out: Albedo, Normal, ORM
	/// 
	/// Step 3: Color
	/// - In: Depth, Albedo, Normal, ORM
	/// - Out: Color
	/// 
	/// Step 4: Post-process
	/// - In: Color
	/// - Out: Post-process
	void CmdDraw(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands, u32 directionalLightCount) const;

private:
	void CreateSampler();

	void CreateImageResources(VkFormat swapChainFormat);
	void CleanupImageResources() const;
	void RegisterFrameContextBufferTextures();
	void UnregisterFrameContextBufferTextures() const;

	UFrameContextAddresses MakeFrameContextAddresses() const;
	UFrameContextTargets MakeFrameContextTargets() const;

	std::vector<UDrawDataBufferData> MakeDrawDataBuffer(std::span<UDrawCommand const> drawCommands) const;
	std::vector<UTransformBufferData> MakeTransformBuffer(std::span<UDrawCommand const> drawCommands) const;
	std::vector<UParametersBufferData> MakeParametersBuffer(std::span<UDrawCommand const> drawCommands) const;

	void CmdUpdateClusterAABB(VkCommandBuffer commandBuffer) const;

	void CmdBarrierComputeFragmentReadToClearWrite(VkCommandBuffer commandBuffer) const;
	void CmdResetLightCounter(VkCommandBuffer commandBuffer) const;
	void CmdBarrierComputeClearWriteToReadWrite(VkCommandBuffer commandBuffer) const;
	void CmdDispatchLightBinning(VkCommandBuffer commandBuffer) const;
	void CmdBarrierComputeWriteToFragmentRead(VkCommandBuffer commandBuffer) const;

	void CmdDrawFrameShadowMaps(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands, u32 directionalLightCount) const;

	void CmdBarrierDepthNoneToWrite(VkCommandBuffer commandBuffer) const;
	void CmdBeginRenderingDepthPrePass(VkCommandBuffer commandBuffer) const;
	void CmdDrawFrameDepthPrePass(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands) const;

	void CmdBarrierDepthWriteToRead(VkCommandBuffer commandBuffer) const;
	void CmdBarrierGBufferNoneToWrite(VkCommandBuffer commandBuffer) const;
	void CmdBeginRenderingGBuffer(VkCommandBuffer commandBuffer) const;
	void CmdDrawFrameGBuffer(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands) const;
	void CmdBarrierGBufferWriteToRead(VkCommandBuffer commandBuffer) const;

	void CmdBarrierDepthReadToShaderRead(VkCommandBuffer commandBuffer) const;
	void CmdBarrierColorNoneToWrite(VkCommandBuffer commandBuffer) const;
	void CmdBeginRenderingDeferredLighting(VkCommandBuffer commandBuffer) const;
	void CmdDrawFrameDeferredLighting(VkCommandBuffer commandBuffer) const;
	void CmdBarrierColorWriteToRead(VkCommandBuffer commandBuffer) const;

	void CmdBarrierPostProcessNoneToWrite(VkCommandBuffer commandBuffer) const;
	void CmdBeginRenderingPostProcess(VkCommandBuffer commandBuffer) const;
	void CmdDrawFramePostProcess(VkCommandBuffer commandBuffer) const;
	void CmdBarrierPostProcessWriteToRead(VkCommandBuffer commandBuffer) const;

	void CmdEndRendering(VkCommandBuffer commandBuffer) const;

	void CmdUseViewport(VkCommandBuffer commandBuffer) const;
	void CmdDrawFrame(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands, u32 directionalIndex) const;
	void CmdPushConstants(VkCommandBuffer commandBuffer, u32 drawIndex, u32 directionalIndex) const;

private:
	VkExtent2D extent_;

	UAllocatedImage depthTarget_;
	UAllocatedImage albedoTarget_;
	UAllocatedImage normalTarget_;
	UAllocatedImage ormTarget_;
	UAllocatedImage colorTarget_;
	UAllocatedImage postProcessTarget_;

	u32 depthIndex_;
	u32 albedoIndex_;
	u32 normalIndex_;
	u32 ormIndex_;
	u32 colorIndex_;
	u32 postProcessIndex_;

	UFrameContextBuffer frameContextBuffer_;

	UDrawDataBuffer drawDataBuffer_;
	UTransformBuffer transformBuffer_;
	UParametersBuffer parametersBuffer_;

	ULightBuffers lightBuffers_;
	UGPUBuffers gpuBuffers_;

	mutable bool isAABBDirty_; // temp fix

	u32 samplerIndex_;
};