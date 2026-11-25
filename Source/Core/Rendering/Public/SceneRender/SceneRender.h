#pragma once

#include "AllocatedBuffer.h"
#include "AllocatedImage.h"
#include "Containers/List.h"
#include "Containers/Span.h"
#include "DrawCommand.h"
#include "FrameContextBuffer/FrameContextBuffer.h"
#include "GPUBuffers/GPUBuffers.h"
#include "LightBuffers/LightBuffers.h"
#include "types.h"

#include <glm/ext/vector_uint2.hpp>
#include <vulkan/vulkan_core.h>

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
	USpan<UDrawCommand const> drawCommands;
	u32 directionalLightCount;
};

class USceneRender final
{
public:
	explicit USceneRender(UDevice& device, USwapchain& swapchain, UPipelineResourceManager& pipelineResourceManager);

	void Init(glm::uvec2 const& extent);
	void Cleanup() const;

	auto GetRenderTarget() const -> u32;
	auto GetDirectionalLightShadowMapTargetIndex(u32 index) const -> u32;

	void UpdateBuffers(UFrameContextSceneView const& sceneView, USpan<UDrawCommand const> drawCommands, USpan<UDirectionalLight const> directionalLights, USpan<UPointLight const> pointLights, u32 samplerIndex) const;

	/**
	 * Step 1: Depth
	 * - Out: Depth
	 * 
	 * Step 2: GBuffer
	 * - In: Depth
	 * - Out: Albedo, Normal, ORM
	 * 
	 * Step 3: Color
	 * - In: Depth, Albedo, Normal, ORM
	 * - Out: Color
	 * 
	 * Step 4: Post-process
	 * - In: Color
	 * - Out: Post-process
	 **/
	void CmdDraw(USceneRenderContext const& renderContext, USceneRenderData const& renderData) const;

private:
	void CreateImageResources(VkFormat depthFormat);
	void CleanupImageResources() const;
	void RegisterFrameContextBufferTextures();
	void UnregisterFrameContextBufferTextures() const;

	auto MakeFrameContextAddresses() const -> UFrameContextAddresses;
	auto MakeFrameContextTargets() const -> UFrameContextTargets;

	auto MakeDrawDataBuffer(USpan<UDrawCommand const> drawCommands) const -> UList<UDrawDataBufferData>;
	auto MakeTransformBuffer(USpan<UDrawCommand const> drawCommands) const -> UList<UTransformBufferData>;
	auto MakeParametersBuffer(USpan<UDrawCommand const> drawCommands) const -> UList<UParametersBufferData>;
	auto MakeMaterialBuffer(USpan<UDrawCommand const> drawCommands) const -> UList<UMaterialBufferData>;

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
	void CmdDrawFrame(VkCommandBuffer commandBuffer, USpan<UDrawCommand const> drawCommands, u32 directionalIndex) const;
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