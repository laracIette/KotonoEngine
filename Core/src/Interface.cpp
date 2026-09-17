#include "Interface.h"

#include "InterfaceRoot.h"
#include <glm/gtx/string_cast.hpp>
#include <kotono_common/log.h>

UInterface::UInterface()
{
}

UInterface::~UInterface()
{
	if (widget_)
	{
		widget_->Delete();
	}
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

void UInterface::Update(f32 deltaTime, glm::vec2 const& cursorPosition)
{
	deltaTime_ = deltaTime;
	now_ += deltaTime;

	UpdateFocusedWidgets(cursorPosition);

	if (widget_)
	{
		widget_->UpdateSceneContexts(deltaTime);
		widget_->Refresh();
	}
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

		KT_LOG(KT_LOG_COMPILE_TIME_LEVEL, "Object", "Main window widget displayed with a size of: {0}", glm::to_string(bounds));
	}
}

void UInterface::EndDraw() const
{
	if (widget_)
	{
		widget_->Remove();

		KT_LOG(KT_LOG_COMPILE_TIME_LEVEL, "Object", "Main window widget removed");
	}
}

void UInterface::OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) const
{
	if (widget_)
	{
		widget_->OnMouseButton(button, inputState, position);
	}
}

void UInterface::OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) const
{
	if (widget_)
	{
		widget_->OnMouseMove(delta, position);
	}
}

void UInterface::OnMouseScroll(glm::vec2 const& delta, glm::vec2 const& position) const
{
	if (widget_)
	{
		widget_->OnMouseScroll(delta, position);
	}
}

void UInterface::OpenWidgetInWindow(UPtr<WWidget> const& widget, glm::uvec2 const& windowExtent)
{
	if (!widget)
	{
		return;
	}

	pendingWindows_.emplace_back(widget, windowExtent);
}

void UInterface::ClearPendingWindows()
{
	pendingWindows_.clear();
}

void UInterface::UpdateFocusedWidgets(glm::vec2 const& cursorPosition)
{
	WidgetSet newFocusedWidgets{};

	if (widget_)
	{
		widget_->PopulateFocusTree(newFocusedWidgets, cursorPosition);
	}

	for (auto const& widget : newFocusedWidgets)
	{
		if (widget && !focusedWidgets_.Contains(widget))
		{
			widget->OnFocused();
		}
	}

	for (auto const& widget : focusedWidgets_)
	{
		if (widget && !newFocusedWidgets.Contains(widget))
		{
			widget->OnUnfocused();
		}
	}

	focusedWidgets_ = newFocusedWidgets;
}
