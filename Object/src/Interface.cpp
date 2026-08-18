#include "Interface.h"

#include "Widget.h"
#include <kotono_common/enum_utils.h>
#include <kotono_graphics/InterfacePendingResources.h>

UInterface::UInterface()
	: timeContext_{
		.frequency = 1.0f / 60.0f,
		.lastDelta = 0.0f,
		.currentDelta = 0.0f,
		.scale = 1.0f,
		.total = 0.0f,
		.state = ETimeContextState::Playing,
	}
{
}

EHandle UInterface::GetTextureHandle(UPath const& path)
{
	auto const it{ textures_.find(path) };
	if (it != textures_.end())
	{
		return it->second;
	}

	auto const handle{ ++currentHandle_ };
	textures_[path] = handle;
	pendingTextures_.push_back({ path, handle });
	return handle;
}

std::span<UPendingTexture const> UInterface::GetPendingTextures() const
{
	return pendingTextures_;
}

void UInterface::ClearPendingTextures()
{
	pendingTextures_.clear();
}

EHandle UInterface::RegisterRenderTarget(glm::uvec2 const& extent)
{
	EHandle handle;

	auto const it{ unusedSceneRenders_.find(extent) };
	if (it != unusedSceneRenders_.end())
	{
		handle = it->second;
		unusedSceneRenders_.erase(it);
	}
	else
	{
		handle = ++currentHandle_;
		pendingSceneRenders_.push_back({ handle, extent });
	}
	sceneRenders_[handle] = extent;
	return handle;
}

void UInterface::UnregisterRenderTarget(EHandle handle)
{
	auto const it{ sceneRenders_.find(handle) };
	if (it != sceneRenders_.end())
	{
		unusedSceneRenders_.insert({ it->second, handle });
		sceneRenders_.erase(it);
	}

	// todo: remove from pending scene render? depends if query each frame instead
}

void UInterface::SetRenderTargetData(EHandle handle, USceneView const& sceneView)
{
	sceneViews_[handle] = sceneView;
}

std::unordered_map<EHandle, USceneView> const& UInterface::GetSceneViews()
{
	return sceneViews_;
}

std::span<UPendingSceneRender const> UInterface::GetPendingSceneRenders() const
{
	return pendingSceneRenders_;
}

std::unordered_multimap<glm::uvec2, EHandle> const& UInterface::GetUnusedSceneRenders() const
{
	return unusedSceneRenders_;
}

void UInterface::ClearPendingSceneRenders()
{
	pendingSceneRenders_.clear();
}

void UInterface::ClearUnusedSceneRenders()
{
	unusedSceneRenders_.clear();
}

void UInterface::Update(f32 deltaTime)
{
	timeContext_.Update(deltaTime);
}

UTimeContext const& UInterface::GetTimeContext() const
{
	return timeContext_;
}
