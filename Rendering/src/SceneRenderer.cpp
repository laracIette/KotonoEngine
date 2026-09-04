#include "SceneRenderer.h"

#include "DrawCommand.h"
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
			sceneRender.Cleanup();
		}
	}
}

USceneRenderer::RenderTarget USceneRenderer::GetSceneRenderTarget(glm::uvec2 const& extent, u32 frameIndex)
{
	auto& frameData{ frameDatas_[frameIndex] };

	SceneRenderHandle handle;

	auto const it{ frameData.availableSceneRenderExtents.find(extent) };
	if (it != frameData.availableSceneRenderExtents.end())
	{
		handle = it->second;
		frameData.availableSceneRenderExtents.erase(it);
	}
	else
	{
		handle = CreateScene(extent);
		frameData.sceneRenderExtents.insert({ extent, handle });
	}

	auto const& sceneRender{ frameData.sceneRenders.at(handle) };
	return sceneRender.GetRenderTarget();
}

USceneRenderer::SceneRenderHandle USceneRenderer::GetSceneRender(glm::uvec2 const& extent, u32 frameIndex)
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
		USceneRender scene{ device_, swapchain_, pipelineResourceManager_ };
		scene.Init(extent);
		frameData.sceneRenders.insert({ currentScene_, scene });
	}

	return currentScene_++;
}

void USceneRenderer::DeleteScene(SceneRenderHandle handle)
{
	for (auto& frameData : frameDatas_)
	{
		auto const& scene{ frameData.sceneRenders.at(handle) };
		scene.Cleanup();
		frameData.sceneRenders.erase(handle);
	}
}

u32 USceneRenderer::GetSceneDirectionalLightShadowMapTargetIndex(u32 frameIndex, SceneRenderHandle handle, u32 index) const
{
	return frameDatas_[frameIndex].sceneRenders.at(handle)
		.GetDirectionalLightShadowMapTargetIndex(index);
}

void USceneRenderer::UpdateSceneBuffers(
	  u32 frameIndex
	, SceneRenderHandle handle
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
	, SceneRenderHandle handle
	, USceneRenderContext const& renderContext
	, USceneRenderData const& renderData
) const
{
	frameDatas_[frameIndex].sceneRenders.at(handle)
		.CmdDraw(renderContext, renderData);
}
