#pragma once
#include "frames_in_flight.h"
#include "SceneRender.h"
#include <glm/ext/vector_uint2.hpp>
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
class UPipelineResourceManager;
class USceneRenderer final
{
public:
	struct FrameData
	{
		std::unordered_map<u32, USceneRender> sceneRenders;
	};

public:
	void Cleanup(VkDevice device, VmaAllocator allocator, UPipelineResourceManager& pipelineResourceManager) const;

	u32 CreateScene(glm::uvec2 const& extent, VkDevice device, VmaAllocator allocator, VkFormat depthFormat, VkFormat swapChainFormat, UPipelineResourceManager& pipelineResourceManager);
	void DeleteScene(u32 handle, VkDevice device, VmaAllocator allocator, UPipelineResourceManager& pipelineResourceManager);

	u32 GetSceneRenderTarget(u32 frameIndex, u32 handle) const;
	u32 GetSceneDirectionalLightShadowMapTargetIndex(u32 frameIndex, u32 handle, u32 index) const;

	void UpdateSceneBuffers(u32 frameIndex, u32 handle, UFrameContextSceneView const& sceneView, std::span<UDrawCommand const> drawCommands, std::span<UDirectionalLight const> directionalLights, std::span<UPointLight const> pointLights, u32 samplerIndex);
	void CmdDrawScene(u32 frameIndex, u32 handle, USceneRenderContext const& renderContext, USceneRenderData const& renderData) const;

private:
	UFramesInFlightArray<FrameData> frameDatas_;
	u32 currentScene_;
};