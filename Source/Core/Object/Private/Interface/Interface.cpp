#include "Interface/Interface.h"

#include "InterfaceRoot/InterfaceRoot.h"
#include <glm/gtx/string_cast.hpp>
#include <Logging/log.h>

UInterface::UInterface() 
	: deltaTime_{ 0.0f }
	, now_{ 0.0f }
{
}

void UInterface::Cleanup() const
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
		widget_->PopulateRenderGraph(interfaceRenderGraph, Colors::White);
	}
}

void UInterface::Update(f32 deltaTime, glm::vec2 const& cursorPosition)
{
	deltaTime_ = deltaTime;
	now_ += deltaTime;

	if (widget_)
	{
		widget_->UpdateSceneContexts(deltaTime);
		widget_->Refresh();
	}

	UpdateFocusedWidgets(cursorPosition);
}

void UInterface::BeginDraw(glm::uvec2 const& bounds)
{
	bounds_ = bounds;
	if (widget_)
	{
		widget_->Display({
			.position = { 0.0f, 0.0f },
			.bounds = glm::vec2{ bounds },
			.scissor{
				.offset = { 0, 0 },
				.extent = bounds
			},
		});

		Log(Normal, "{0} displayed with a size of: {1}", widget_->GetName(), glm::to_string(bounds));
	}
}

void UInterface::EndDraw() const
{
	if (widget_)
	{
		widget_->Remove();

		Log(Normal, "{0} removed", widget_->GetName());
	}
}

void UInterface::OnMouseButton(EButton button, EInputState inputState, EModifier modifier)
{
	if (inputState == EInputState::Pressed)
	{
		UpdateActiveWidgets();
	}

	for (auto const& widget : activeWidgets_ | std::views::reverse)
	{
		if (widget && widget->OnMouseButton(button, inputState, modifier) == INPUT_HANDLED)
		{
			break;
		}
	}
}

void UInterface::OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) const
{
	for (auto const& widget : activeWidgets_ | std::views::reverse)
	{
		if (widget && widget->OnMouseMove(delta, position) == INPUT_HANDLED)
		{
			break;
		}
	}
}

void UInterface::OnMouseScroll(glm::vec2 const& delta) const
{
	for (auto const& widget : activeWidgets_ | std::views::reverse)
 	{
 		if (widget && widget->OnMouseScroll(delta) == INPUT_HANDLED)
 		{
 			break;
 		}
 	}
}

void UInterface::OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) const
{
	for (auto const& widget : activeWidgets_ | std::views::reverse)
	{
		if (widget && widget->OnKeyboardKey(key, inputState, modifier) == INPUT_HANDLED)
		{
			break;
		}
	}
}

void UInterface::OpenWidgetInWindow(glm::uvec2 const& windowExtent, UPtr<WWidget> const& widget, std::string_view name)
{
	if (!widget)
	{
		return;
	}

	pendingWindows_.emplace_back(windowExtent, widget, std::string{ name });
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

	for (auto const& widget : focusedWidgets_)
	{
		if (!widget)
		{
			Log(Warning, "widget is null");
			continue;
		}
		
		if (!newFocusedWidgets.Contains(widget))
		{
			widget->SetUnfocused();
		}
	}

	for (auto const& widget : newFocusedWidgets)
	{
		if (!widget)
		{
			Log(Warning, "widget is null");
			continue;
		}
		
		if (!focusedWidgets_.Contains(widget))
		{
			widget->SetFocused();
		}
	}

	focusedWidgets_ = newFocusedWidgets;
}

void UInterface::UpdateActiveWidgets()
{
	for (auto const& widget : activeWidgets_)
	{
		if (!widget)
		{
			Log(Warning, "widget is null");
			continue;
		}
			
		if (!focusedWidgets_.Contains(widget))
		{
			widget->SetInactive();
		}
	}

	for (auto const& widget : focusedWidgets_)
	{
		if (!widget)
		{
			Log(Warning, "widget is null");
			continue;
		}
		
		if (!activeWidgets_.Contains(widget))
		{
			widget->SetActive();
		}
	}
	
	activeWidgets_ = focusedWidgets_;
}
