#include "SceneRenderer.h"

#include "DrawCommand.h"
#include <algorithm>
#include <array>
#include <ranges>

void USceneRenderer::Cleanup() const
{
	for (auto const& frameData : frameDatas_)
	{
		std::ranges::for_each(frameData.sceneRenders | std::views::values, &USceneRender::Cleanup);
	}
}

u32 USceneRenderer::CreateScene(glm::uvec2 const& extent, VkFormat swapChainFormat)
{
	for (auto& frameData : frameDatas_)
	{
		USceneRender scene{};
		scene.Init(extent, swapChainFormat);
		frameData.sceneRenders[currentScene_] = scene;
	}

	return currentScene_++;
}

void USceneRenderer::DeleteScene(u32 handle)
{
	for (auto& frameData : frameDatas_)
	{
		auto const& scene{ frameData.sceneRenders.at(handle) };
		scene.Cleanup();
		frameData.sceneRenders.erase(handle);
	}
}

void USceneRenderer::SetSceneExtent(u32 handle, glm::uvec2 const& extent, VkFormat swapChainFormat)
{
	for (auto& frameData : frameDatas_)
	{
		auto& scene{ frameData.sceneRenders.at(handle) };
		scene.SetExtent(extent, swapChainFormat);
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
)
{
	auto const& scene{ frameDatas_[frameIndex].sceneRenders.at(handle) };
	scene.UpdateBuffers(sceneView, drawCommands, directionalLights, pointLights);
}

void USceneRenderer::CmdDrawScene(VkCommandBuffer commandBuffer, u32 frameIndex, u32 handle, std::span<UDrawCommand const> drawCommands, u32 directionalLightCount) const
{
	auto const& scene{ frameDatas_[frameIndex].sceneRenders.at(handle) };
	scene.CmdDraw(commandBuffer, drawCommands, directionalLightCount);
}
