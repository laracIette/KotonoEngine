#pragma once

#include "Containers/Map.h"
#include "Containers/Span.h"
#include "frames_in_flight.h"
#include "IndexBuffer/IndexBuffer.h"
#include "InterfaceRenderer/InterfaceRenderer.h"
#include "PipelineResourceManager/PipelineResourceManager.h"
#include "RenderGraph/InterfaceDrawData.h"
#include "SceneRenderer/SceneRenderer.h"
#include "Swapchain/Swapchain.h"
#include "Path/Path.h"
#include "types.h"

#include <vector>
#include <vulkan/vulkan_core.h>

struct UDirectionalLight;
struct UDirectionalLightData;
struct UDrawCommand;
struct UFrameContextSceneView;
struct UInterfaceRenderGraph;
struct UPointLight;
struct UPointLightData;
struct USceneRenderView;
class AMaterial;
class AModel;
class ASampler;
class AShader;
class ATexture;
class UDevice;
class USurface;

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
	};

public:
	explicit URenderer(UDevice& device, USurface& surface);

	void Init();
	void Cleanup();

	void DrawFrame(UInterfaceRenderGraph const& interfaceRenderGraph);

private:
	void InitSceneRendererResources();

	void RecreateFrames();

	b8 TryAcquireNextImage(u32 frameIndex);

	void CreateCommandPools();
	void CreateCommandPool(u32 frameIndex);
	void CreateCommandBuffers();
	void CreateCommandBuffer(u32 frameIndex);
	void RecordCommandBuffer(u32 frameIndex, USpan<USceneRenderView const> sceneRenderViews, USpan<UDrawCommand const> interfaceDrawCommands) const;
	void BeginCommandBuffer(VkCommandBuffer commandBuffer) const;

	void CmdBarrierSwapchainNoneToWrite(VkCommandBuffer commandBuffer, u32 frameIndex) const;
	void CmdBeginRenderingInterface(VkCommandBuffer commandBuffer, u32 frameIndex) const;
	void CmdBarrierSwapchainWriteToPresent(VkCommandBuffer commandBuffer, u32 frameIndex) const;
	void CmdEndRendering(VkCommandBuffer commandBuffer) const;
	
	void EndCommandBuffer(VkCommandBuffer commandBuffer) const;
	void SubmitCommandBuffer(u32 frameIndex);

	void CreateSyncObjects();

	auto MakeFrameContextSceneView(USceneView const& sceneView) const -> UFrameContextSceneView;
	auto MakeInterfaceDrawCommands(USpan<UInterfaceDrawData const> drawDatas, u32 frameIndex) -> std::vector<UDrawCommand>;
	auto MakeSceneDrawCommands(USpan<USceneDrawData const> drawDatas, u32 frameIndex) -> std::vector<UDrawCommand>;
	auto MakeDirectionalLights(USpan<UDirectionalLightData const> directionalLightDatas, UFrameContextSceneView const& sceneView, USceneRenderer::SceneRenderHandle sceneRender, u32 frameIndex) -> std::vector<UDirectionalLight>;
	auto MakePointLights(USpan<UPointLightData const> pointLightDatas) const -> std::vector<UPointLight>;
	auto MakeSceneRenderViews(USpan<UInterfaceDrawData const> drawDatas, u32 frameIndex) -> std::vector<USceneRenderView>;

	auto GetOrCreateTexture(UPath const& path) -> ATexture*;
	auto GetOrCreateMaterial(UPath const& path) -> AMaterial*;
	auto GetOrCreateSampler(UPath const& path) -> ASampler*;
	auto GetOrCreateModel(UPath const& path) -> AModel*;
	auto GetOrCreateShader(UPath const& path) -> AShader*;

	auto GetTextureHandle(UInterfaceDrawData::Texture const& texture, u32 frameIndex) -> u32;

private:
	UDevice& device_;

	USwapchain swapchain_;
	UFramesInFlightArray<FrameData> frameDatas_;

	u32 frameCount_;

	USceneRenderer sceneRenderer_;
	UInterfaceRenderer interfaceRenderer_;

	UPipelineResourceManager pipelineResourceManager_;
	UIndexBuffer indexBuffer_;

	VkPipeline clusterAABBPipeline_;
	VkPipeline lightBinningPipeline_;
	VkPipeline shadowPrePassPipeline_;
	VkPipeline depthPrePassPipeline_;
	VkPipeline deferredLightingPipeline_;
	VkPipeline postProcessPipeline_;
	u32 defaultSampler_;

	UMap<UPath, ATexture*> textures_;
	UMap<UPath, AMaterial*> materials_;
	UMap<UPath, ASampler*> samplers_;
	UMap<UPath, AModel*> models_;
	UMap<UPath, AShader*> shaders_;
};
