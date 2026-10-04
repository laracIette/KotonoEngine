#pragma once
#include "FrameContextBuffer.h"
#include "GPUBuffers.h"
#include "LightBuffers.h"
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedBuffer.h>
#include <kotono_platform/AllocatedImage.h>
#include <span>
#include <vector>
#include <vulkan/vulkan_core.h>
struct UDrawCommand;
struct UFrameContextSceneView;
struct UDrawDataBufferData;
struct UTransformBufferData;
struct UParametersBufferData;
struct UMaterialBufferData;
class UDevice;
class UIndexBuffer;
class UPipelineResourceManager;
class USwapchain;
struct USceneRenderContext final
{
	VkCommandBuffer	commandBuffer;

	VkPipeline clusterAABBPipeline;
	VkPipeline lightBinningPipeline;
	VkPipeline shadowPrePassPipeline;
	VkPipeline depthPrePassPipeline;
	VkPipeline deferredLightingPipeline;
	VkPipeline postProcessPipeline;

	UIndexBuffer const& indexBuffer;
};
struct USceneRenderData final
{
	std::span<UDrawCommand const> drawCommands;
	u32 directionalLightCount;
};
class USceneRender final
{
public:
	explicit USceneRender(UDevice& device, USwapchain& swapchain, UPipelineResourceManager& pipelineResourceManager);

	void Init(glm::uvec2 const& extent);
	void Cleanup() const;

	u32 GetRenderTarget() const;
	u32 GetDirectionalLightShadowMapTargetIndex(u32 index) const;

	void UpdateBuffers(UFrameContextSceneView const& sceneView, std::span<UDrawCommand const> drawCommands, std::span<UDirectionalLight const> directionalLights, std::span<UPointLight const> pointLights, u32 samplerIndex) const;

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
	void CmdDraw(USceneRenderContext const& renderContext, USceneRenderData const& renderData) const;

private:
	void CreateImageResources(VkFormat depthFormat);
	void CleanupImageResources() const;
	void RegisterFrameContextBufferTextures();
	void UnregisterFrameContextBufferTextures() const;

	UFrameContextAddresses MakeFrameContextAddresses() const;
	UFrameContextTargets MakeFrameContextTargets() const;

	std::vector<UDrawDataBufferData> MakeDrawDataBuffer(std::span<UDrawCommand const> drawCommands) const;
	std::vector<UTransformBufferData> MakeTransformBuffer(std::span<UDrawCommand const> drawCommands) const;
	std::vector<UParametersBufferData> MakeParametersBuffer(std::span<UDrawCommand const> drawCommands) const;
	std::vector<UMaterialBufferData> MakeMaterialBuffer(std::span<UDrawCommand const> drawCommands) const;

	void CmdUpdateClusterAABB(USceneRenderContext const& renderContext) const;

	void CmdBarrierComputeFragmentReadToClearWrite(VkCommandBuffer commandBuffer) const;
	void CmdResetLightCounter(VkCommandBuffer commandBuffer) const;
	void CmdBarrierComputeClearWriteToReadWrite(VkCommandBuffer commandBuffer) const;
	void CmdDispatchLightBinning(USceneRenderContext const& renderContext) const;
	void CmdBarrierComputeWriteToFragmentRead(VkCommandBuffer commandBuffer) const;

	void CmdDrawFrameShadowMaps(USceneRenderContext const& renderContext, USceneRenderData const& renderData) const;

	void CmdBarrierDepthNoneToWrite(VkCommandBuffer commandBuffer) const;
	void CmdBeginRenderingDepthPrePass(VkCommandBuffer commandBuffer) const;
	void CmdDrawFrameDepthPrePass(USceneRenderContext const& renderContext, USceneRenderData const& renderData) const;

	void CmdBarrierDepthWriteToRead(VkCommandBuffer commandBuffer) const;
	void CmdBarrierGBufferNoneToWrite(VkCommandBuffer commandBuffer) const;
	void CmdBeginRenderingGBuffer(VkCommandBuffer commandBuffer) const;
	void CmdDrawFrameGBuffer(USceneRenderContext const& renderContext, USceneRenderData const& renderData) const;
	void CmdBarrierGBufferWriteToRead(VkCommandBuffer commandBuffer) const;

	void CmdBarrierDepthReadToShaderRead(VkCommandBuffer commandBuffer) const;
	void CmdBarrierColorNoneToWrite(VkCommandBuffer commandBuffer) const;
	void CmdBeginRenderingDeferredLighting(VkCommandBuffer commandBuffer) const;
	void CmdDrawFrameDeferredLighting(USceneRenderContext const& renderContext) const;
	void CmdBarrierColorWriteToRead(VkCommandBuffer commandBuffer) const;

	void CmdBarrierPostProcessNoneToWrite(VkCommandBuffer commandBuffer) const;
	void CmdBeginRenderingPostProcess(VkCommandBuffer commandBuffer) const;
	void CmdDrawFramePostProcess(USceneRenderContext const& renderContext) const;
	void CmdBarrierPostProcessWriteToRead(VkCommandBuffer commandBuffer) const;

	void CmdEndRendering(VkCommandBuffer commandBuffer) const;

	void CmdUseViewport(VkCommandBuffer commandBuffer) const;
	void CmdDrawFrame(VkCommandBuffer commandBuffer, std::span<UDrawCommand const> drawCommands, u32 directionalIndex) const;
	void CmdPushConstants(VkCommandBuffer commandBuffer, u32 drawIndex, u32 directionalIndex) const;

private:
	UDevice& device_;
	USwapchain& swapchain_;
	UPipelineResourceManager& pipelineResourceManager_;

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

	UAllocatedBuffer drawDataBuffer_;
	UAllocatedBuffer transformBuffer_;
	UAllocatedBuffer parametersBuffer_;
	UAllocatedBuffer materialBuffer_;

	ULightBuffers lightBuffers_;
	UGPUBuffers gpuBuffers_;

	mutable bool isAABBDirty_; // todo: temp fix
};