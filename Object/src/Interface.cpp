#include "Interface.h"

#include "SceneContext.h"
#include <kotono_common/enum_utils.h>
#include <kotono_graphics/InterfacePendingResources.h>
#include <kotono_input/Mouse.h>

UInterface::UInterface()
	: timeContext_{
		.frequency = 1.0f / 60.0f,
		.scale = 1.0f,
		.state = ETimeContextState::Playing,
	}
{
}

UInterface::~UInterface()
{
	if (widget_)
	{
		widget_->Delete();
	}
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

void UInterface::ClearPendingSceneRenders()
{
	pendingSceneRenders_.clear();
}

void UInterface::ClearUnusedSceneRenders()
{
	unusedSceneRenders_.clear();
}

void UInterface::PopulateInterfaceRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	if (widget_)
	{
		widget_->PopulateRenderGraph(interfaceRenderGraph);
	}
}

void UInterface::PopulateSceneRenderGraph(USceneRenderGraph& sceneRenderGraph) const
{
	if (widget_)
	{
		widget_->PopulateSceneRenderGraph(sceneRenderGraph);
	}
}

void UInterface::Update(f32 deltaTime)
{
	timeContext_.Update(deltaTime);

	if (!widget_)
	{
		return;
	}

	widget_->Update(deltaTime);

	WidgetSet focusedWidgets{};
	widget_->PopulateFocusTree(focusedWidgets, Mouse.GetCursorPosition());

	for (auto const& widget : focusedWidgets)
	{
		if (!focusedWidgets_.Contains(widget))
		{
			widget->OnFocused();
		}
	}

	for (auto const& widget : focusedWidgets_)
	{
		if (widget)
		{
			if (!focusedWidgets.Contains(widget))
			{
				widget->OnUnfocused();
			}
		}
	}

	focusedWidgets_ = focusedWidgets;
}

void UInterface::BeginDraw(glm::uvec2 const& bounds)
{
	bounds_ = bounds;
	if (widget_)
	{
		widget_->Display({
			.position = { 0.0f, 0.0f },
			.bounds = glm::vec2{ bounds },
			.layer = 0,
			.scissor{
				.offset = { 0, 0 },
				.extent = bounds
			},
		});
	}
}

void UInterface::EndDraw() const
{
	if (widget_)
	{
		widget_->Remove();
	}
}

std::span<UPendingTexture const> UInterface::GetPendingTextures() const
{ 
	return pendingTextures_; 
}

std::span<UPendingSceneRender const> UInterface::GetPendingSceneRenders() const
{ 
	return pendingSceneRenders_;
}

