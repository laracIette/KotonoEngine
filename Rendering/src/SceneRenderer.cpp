#include "SceneRenderer.h"

#include "DrawCommand.h"
#include <algorithm>
#include <array>
#include <ranges>

USceneRenderer::USceneRenderer(UDevice& device, USwapchain& swapchain, UPipelineResourceManager& pipelineResourceManager)
	: device_{ device }
	, swapchain_{ swapchain }
	, pipelineResourceManager_{ pipelineResourceManager }
{
}

void USceneRenderer::Cleanup() const
{
	for (auto const& sceneRenders : frameDatas_ | std::views::transform(&FrameData::sceneRenders))
	{
		for (auto const& sceneRender : sceneRenders | std::views::values)
		{
			sceneRender.Cleanup(pipelineResourceManager_);
		}
	}
}

u32 USceneRenderer::GetSceneRenderTarget(glm::uvec2 const& extent, u32 frameIndex)
{
	auto& frameData{ frameDatas_[frameIndex] };

	u32 handle;

	auto const it{ frameData.sceneRenderExtents.find(extent) };
	if (it != frameData.sceneRenderExtents.end())
	{
		handle = it->second;
	}
	else
	{
		handle = CreateScene(extent);
		frameData.sceneRenderExtents.insert({ extent, handle });
	}

	auto const& sceneRender{ frameData.sceneRenders.at(handle) };
	return sceneRender.GetRenderTarget();
}

u32 USceneRenderer::GetSceneRender(glm::uvec2 const& extent, u32 frameIndex)
{
	auto& frameData{ frameDatas_[frameIndex] };

	auto const it{ frameData.availableSceneRenderExtents.find(extent) };
	if (it != frameData.availableSceneRenderExtents.end())
	{
		auto const handle{ it->second };
		frameData.availableSceneRenderExtents.erase(it);
		return handle;
	}
	
	throw std::runtime_error{ "couldn't find an available scene render!" };
}

void USceneRenderer::RefreshAvailableSceneRenders(u32 frameIndex)
{
	auto& frameData{ frameDatas_[frameIndex] };
	frameData.availableSceneRenderExtents = frameData.sceneRenderExtents;
}

void USceneRenderer::ClearUnusedSceneRenders(u32 frameIndex)
{
	auto& frameData{ frameDatas_[frameIndex] };

	for (auto const& [extent, handle] : frameData.availableSceneRenderExtents)
	{
		auto const it{ frameData.sceneRenderExtents.find(extent) };
		if (it != frameData.sceneRenderExtents.end())
		{
			frameData.sceneRenderExtents.erase(it);
		}

		DeleteScene(handle);
	}
}

u32 USceneRenderer::CreateScene(glm::uvec2 const& extent)
{
	for (auto& frameData : frameDatas_)
	{
		USceneRender scene{ device_, swapchain_ };
		scene.Init(extent, pipelineResourceManager_);
		frameData.sceneRenders.insert({ currentScene_, scene });
	}

	return currentScene_++;
}

void USceneRenderer::DeleteScene(u32 handle)
{
	for (auto& frameData : frameDatas_)
	{
		auto const& scene{ frameData.sceneRenders.at(handle) };
		scene.Cleanup(pipelineResourceManager_);
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
