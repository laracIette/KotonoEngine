#pragma once
#include "frames_in_flight.h"
#include "SceneRender.h"
#include <glm/ext/vector_uint2.hpp>
#include <glm/gtx/hash.hpp>
#include <kotono_common/types.h>
#include <span>
#include <unordered_map>
#include <vulkan/vulkan_core.h>
struct UDrawCommand;
struct UDirectionalLight;
struct UPointLight;
struct UFrameContextSceneView;
struct USceneRenderContext;
struct USceneRenderData;
class UDevice;
class UPipelineResourceManager;
class USwapchain;
class USceneRenderer final
{
public:
	struct FrameData
	{
		std::unordered_map<u32, USceneRender> sceneRenders;
		std::unordered_multimap<glm::uvec2, u32> sceneRenderExtents;
		std::unordered_multimap<glm::uvec2, u32> availableSceneRenderExtents;
	};

public:
	explicit USceneRenderer(UDevice& device, USwapchain& swapchain, UPipelineResourceManager& pipelineResourceManager);

	void Cleanup() const;

	u32 GetSceneRenderTarget(glm::uvec2 const& extent, u32 frameIndex);
	u32 GetSceneRender(glm::uvec2 const& extent, u32 frameIndex);
	void RefreshAvailableSceneRenders(u32 frameIndex);
	void ClearUnusedSceneRenders(u32 frameIndex);

	u32 CreateScene(glm::uvec2 const& extent);
	void DeleteScene(u32 handle);

	u32 GetSceneRenderTarget(u32 frameIndex, u32 handle) const;
	u32 GetSceneDirectionalLightShadowMapTargetIndex(u32 frameIndex, u32 handle, u32 index) const;

	void UpdateSceneBuffers(u32 frameIndex, u32 handle, UFrameContextSceneView const& sceneView, std::span<UDrawCommand const> drawCommands, std::span<UDirectionalLight const> directionalLights, std::span<UPointLight const> pointLights, u32 samplerIndex);
	void CmdDrawScene(u32 frameIndex, u32 handle, USceneRenderContext const& renderContext, USceneRenderData const& renderData) const;

private:
	UDevice& device_;
	USwapchain& swapchain_;
	UPipelineResourceManager& pipelineResourceManager_;

	UFramesInFlightArray<FrameData> frameDatas_;
	u32 currentScene_;
};