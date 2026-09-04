#pragma once
#include "FrameContextBuffer.h"
#include "frames_in_flight.h"
#include "GPUBuffers.h"
#include "IndexBuffer.h"
#include "InterfaceRenderer.h"
#include "LightBuffers.h"
#include "PipelineResourceManager.h"
#include "PushConstants.h"
#include "SceneRenderer.h"
#include <flat_map>
#include <kotono_common/Handle.h>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
#include <kotono_platform/AllocatedImage.h>
#include <kotono_platform/Swapchain.h>
#include <span>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>
struct UDirectionalLight;
struct UDirectionalLightData;
struct UDrawCommand;
struct UDrawData;
struct UInterfaceRenderGraph;
struct UPointLight;
struct UPointLightData;
struct USceneRenderGraph;
struct USceneView;
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

	struct SceneRenderView
	{
		u32 sceneRender;
		UFrameContextSceneView sceneView;
	};

public:
	explicit URenderer(UDevice& device, USurface& surface);

	void Init();
	void Cleanup();

	void UnregisterUnusedSceneRenders();

	void DrawFrame(USceneRenderGraph const& sceneRenderGraph, UInterfaceRenderGraph const& interfaceRenderGraph);

private:
	void InitSceneRendererResources();

	void RecreateFrames();

	b8 TryAcquireNextImage(u32 frameIndex);

	void CreateCommandPools();
	void CreateCommandPool(u32 frameIndex);
	void CreateCommandBuffers();
	void CreateCommandBuffer(u32 frameIndex);
	void RecordCommandBuffer(u32 frameIndex, std::span<SceneRenderView const> sceneRenderViews, std::span<UDrawCommand const> sceneDrawCommands, std::span<UDrawCommand const> interfaceDrawCommands, u32 directionalLightCount) const;
	void BeginCommandBuffer(VkCommandBuffer commandBuffer) const;

	void CmdBarrierSwapchainNoneToWrite(VkCommandBuffer commandBuffer, u32 frameIndex) const;
	void CmdBeginRenderingInterface(VkCommandBuffer commandBuffer, u32 frameIndex) const;
	void CmdBarrierSwapchainWriteToPresent(VkCommandBuffer commandBuffer, u32 frameIndex) const;
	void CmdEndRendering(VkCommandBuffer commandBuffer) const;
	
	void EndCommandBuffer(VkCommandBuffer commandBuffer) const;
	void SubmitCommandBuffer(u32 frameIndex);

	void CreateSyncObjects();

	u32 GetGameThreadFrame() const;
	u32 GetRenderThreadFrame() const;
	u32 GetRHIThreadFrame() const;

	UFrameContextSceneView MakeFrameContextSceneView(USceneView const& sceneView) const;
	std::vector<UDrawCommand> MakeDrawCommands(std::span<UDrawData const> drawDatas, u32 frameIndex);
	std::vector<UDirectionalLight> MakeDirectionalLights(std::span<UDirectionalLightData const> directionalLightDatas, UFrameContextSceneView const& sceneView, u32 sceneRender, u32 frameIndex);
	std::vector<UPointLight> MakePointLights(std::span<UPointLightData const> pointLightDatas) const;

	ATexture* GetOrCreateTexture(UPath const& path);
	AMaterial* GetOrCreateMaterial(UPath const& path);
	ASampler* GetOrCreateSampler(UPath const& path);
	AModel* GetOrCreateModel(UPath const& path);
	AShader* GetOrCreateShader(UPath const& path);

	u32 GetTextureHandle(std::variant<UPath, USceneView> const& texture, u32 frameIndex);

private:
	UDevice& device_;

	USwapchain swapchain_;
	UFramesInFlightArray<FrameData> frameDatas_;

	std::thread renderThread_;
	std::thread rhiThread_;

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
