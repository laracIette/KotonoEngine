#include "Interface.h"

#include "SceneContext.h"
#include <glm/gtx/string_cast.hpp>
#include <kotono_common/log.h>
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
	if (timeContext_.Update(deltaTime))
	{
	}

	if (widget_)
	{
		widget_->Update(deltaTime);
	}

	UpdateFocusedWidgets();
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

void UInterface::UpdateFocusedWidgets()
{
	WidgetSet focusedWidgets{};

	if (widget_)
	{
		widget_->PopulateFocusTree(focusedWidgets, Mouse.GetCursorPosition());
	}

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
