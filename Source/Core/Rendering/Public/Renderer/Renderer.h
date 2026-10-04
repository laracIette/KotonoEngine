#pragma once
#include "frames_in_flight.h"
#include "IndexBuffer.h"
#include "InterfaceRenderer.h"
#include "PipelineResourceManager.h"
#include "SceneRenderer.h"
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
#include <kotono_graphics/InterfaceDrawData.h>
#include <kotono_platform/Swapchain.h>
#include <span>
#include <unordered_map>
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
	void RecordCommandBuffer(u32 frameIndex, std::span<USceneRenderView const> sceneRenderViews, std::span<UDrawCommand const> interfaceDrawCommands) const;
	void BeginCommandBuffer(VkCommandBuffer commandBuffer) const;

	void CmdBarrierSwapchainNoneToWrite(VkCommandBuffer commandBuffer, u32 frameIndex) const;
	void CmdBeginRenderingInterface(VkCommandBuffer commandBuffer, u32 frameIndex) const;
	void CmdBarrierSwapchainWriteToPresent(VkCommandBuffer commandBuffer, u32 frameIndex) const;
	void CmdEndRendering(VkCommandBuffer commandBuffer) const;
	
	void EndCommandBuffer(VkCommandBuffer commandBuffer) const;
	void SubmitCommandBuffer(u32 frameIndex);

	void CreateSyncObjects();

	auto MakeFrameContextSceneView(USceneView const& sceneView) const -> UFrameContextSceneView;
	auto MakeInterfaceDrawCommands(std::span<UInterfaceDrawData const> drawDatas, u32 frameIndex) -> std::vector<UDrawCommand>;
	auto MakeSceneDrawCommands(std::span<USceneDrawData const> drawDatas, u32 frameIndex) -> std::vector<UDrawCommand>;
	auto MakeDirectionalLights(std::span<UDirectionalLightData const> directionalLightDatas, UFrameContextSceneView const& sceneView, USceneRenderer::SceneRenderHandle sceneRender, u32 frameIndex) -> std::vector<UDirectionalLight>;
	auto MakePointLights(std::span<UPointLightData const> pointLightDatas) const -> std::vector<UPointLight>;
	auto MakeSceneRenderViews(std::span<UInterfaceDrawData const> drawDatas, u32 frameIndex) -> std::vector<USceneRenderView>;

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

	std::unordered_map<UPath, ATexture*> textures_;
	std::unordered_map<UPath, AMaterial*> materials_;
	std::unordered_map<UPath, ASampler*> samplers_;
	std::unordered_map<UPath, AModel*> models_;
	std::unordered_map<UPath, AShader*> shaders_;
};
