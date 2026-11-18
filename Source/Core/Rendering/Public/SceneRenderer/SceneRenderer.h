#pragma once

#include "Containers/Span.h"
#include "frames_in_flight.h"
#include "SceneRender/SceneRender.h"
#include "types.h"

#include <glm/ext/vector_uint2.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

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
	using SceneRenderHandle = u32;
	using RenderTarget = u32;

public:
	struct FrameData
	{
		std::unordered_map<SceneRenderHandle, USceneRender> sceneRenders;
		std::unordered_multimap<glm::uvec2, SceneRenderHandle> sceneRenderExtents;
		std::unordered_multimap<glm::uvec2, SceneRenderHandle> availableSceneRenderExtents;
	};

public:
	explicit USceneRenderer(UDevice& device, USwapchain& swapchain, UPipelineResourceManager& pipelineResourceManager);

	void Cleanup() const;

	auto GetSceneRenderTarget(glm::uvec2 const& extent, u32 frameIndex) -> RenderTarget;
	auto GetSceneRender(glm::uvec2 const& extent, u32 frameIndex) -> SceneRenderHandle;
	void RefreshAvailableSceneRenders(u32 frameIndex);
	void ClearUnusedSceneRenders(u32 frameIndex);

	auto GetSceneDirectionalLightShadowMapTargetIndex(u32 frameIndex, SceneRenderHandle handle, u32 index) const -> u32;

	void UpdateSceneBuffers(u32 frameIndex, SceneRenderHandle handle, UFrameContextSceneView const& sceneView, USpan<UDrawCommand const> drawCommands, USpan<UDirectionalLight const> directionalLights, USpan<UPointLight const> pointLights, u32 samplerIndex) const;
	void CmdDrawScene(u32 frameIndex, SceneRenderHandle handle, USceneRenderContext const& renderContext, USceneRenderData const& renderData) const;

private:
	auto CreateScene(glm::uvec2 const& extent) -> SceneRenderHandle;
	void DeleteScene(SceneRenderHandle handle);

private:
	UDevice& device_;
	USwapchain& swapchain_;
	UPipelineResourceManager& pipelineResourceManager_;

	UFramesInFlightArray<FrameData> frameDatas_;
	SceneRenderHandle currentScene_;
};