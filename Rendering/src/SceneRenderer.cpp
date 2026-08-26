#include "SceneRenderer.h"

#include "DrawCommand.h"
#include <algorithm>
#include <array>
#include <ranges>

void USceneRenderer::Cleanup(
	  VkDevice device
	, VmaAllocator allocator
	, UPipelineResourceManager& pipelineResourceManager
) const
{
	for (auto const& sceneRenders : frameDatas_ | std::views::transform(&FrameData::sceneRenders))
	{
		for (auto const& sceneRender : sceneRenders | std::views::values)
		{
			sceneRender.Cleanup(device, allocator, pipelineResourceManager);
		}
	}
}

u32 USceneRenderer::CreateScene(
	  glm::uvec2 const& extent
	, VkDevice device
	, VmaAllocator allocator
	, VkFormat depthFormat
	, VkFormat swapChainFormat
	, UPipelineResourceManager& pipelineResourceManager
)
{
	for (auto& frameData : frameDatas_)
	{
		USceneRender scene{};
		scene.Init(extent, device, allocator, depthFormat, swapChainFormat, pipelineResourceManager);
		frameData.sceneRenders[currentScene_] = scene;
	}

	return currentScene_++;
}

void USceneRenderer::DeleteScene(
	  u32 handle
	, VkDevice device
	, VmaAllocator allocator
	, UPipelineResourceManager& pipelineResourceManager
)
{
	for (auto& frameData : frameDatas_)
	{
		auto const& scene{ frameData.sceneRenders.at(handle) };
		scene.Cleanup(device, allocator, pipelineResourceManager);
		frameData.sceneRenders.erase(handle);
	}
}

u32 USceneRenderer::GetSceneRenderTarget(u32 frameIndex, u32 handle) const
{
	return frameDatas_[frameIndex].sceneRenders.at(handle)
		.GetRenderTarget();
}

u32 USceneRenderer::GetSceneDirectionalLightShadowMapTargetIndex(u32 frameIndex, u32 handle, u32 index) const
{
	return frameDatas_[frameIndex].sceneRenders.at(handle)
		.GetDirectionalLightShadowMapTargetIndex(index);
}

void USceneRenderer::UpdateSceneBuffers(
	  u32 frameIndex
	, u32 handle
	, UFrameContextSceneView const& sceneView
	, std::span<UDrawCommand const> drawCommands
	, std::span<UDirectionalLight const> directionalLights
	, std::span<UPointLight const> pointLights
	, u32 samplerIndex
)
{
	frameDatas_[frameIndex].sceneRenders.at(handle)
		.UpdateBuffers(sceneView, drawCommands, directionalLights, pointLights, samplerIndex);
}

void USceneRenderer::CmdDrawScene(
	  u32 frameIndex
	, u32 handle
	, USceneRenderContext const& renderContext
	, USceneRenderData const& renderData
) const
{
	frameDatas_[frameIndex].sceneRenders.at(handle)
		.CmdDraw(renderContext, renderData);
}
