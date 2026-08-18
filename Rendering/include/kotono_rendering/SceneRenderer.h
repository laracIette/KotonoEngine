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
class USceneRenderer final
{
public:
	struct FrameData
	{
		std::unordered_map<u32, USceneRender> sceneRenders;
	};

public:
	void Cleanup() const;

	u32 CreateScene(glm::uvec2 const& extent, VkFormat swapChainFormat);
	void DeleteScene(u32 handle);

	void SetSceneExtent(u32 handle, glm::uvec2 const& extent, VkFormat swapChainFormat);

	u32 GetSceneRenderTarget(u32 frameIndex, u32 handle) const;
	u32 GetSceneDirectionalLightShadowMapTargetIndex(u32 frameIndex, u32 handle, u32 index) const;

	void UpdateSceneBuffers(u32 frameIndex, u32 handle, UFrameContextSceneView const& sceneView, std::span<UDrawCommand const> drawCommands, std::span<UDirectionalLight const> directionalLights, std::span<UPointLight const> pointLights);
	void CmdDrawScene(VkCommandBuffer commandBuffer, u32 frameIndex, u32 handle, std::span<UDrawCommand const> drawCommands, u32 directionalLightCount) const;

private:
	UFramesInFlightArray<FrameData> frameDatas_;
	u32 currentScene_;
};