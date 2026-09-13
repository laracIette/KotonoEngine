#include "Interface.h"

#include "InterfaceRoot.h"
#include <glm/gtx/string_cast.hpp>
#include <kotono_common/log.h>
#include <kotono_input/Mouse.h>

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

void UInterface::Update(f32 deltaTime)
{
	deltaTime_ = deltaTime;
	now_ += deltaTime;

	if (widget_)
	{
		widget_->UpdateSceneContexts(deltaTime);
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

		Mouse.GetEventAnyButton().AddListener(widget_.Get(), &WInterfaceRoot::OnMouseButton);
		Mouse.GetEventMove().AddListener(widget_.Get(), &WInterfaceRoot::OnMouseMove);

		KT_LOG(KT_LOG_COMPILE_TIME_LEVEL, "Object", "Main window widget displayed with a size of: {0}", glm::to_string(bounds));
	}
}

void UInterface::EndDraw() const
{
	if (widget_)
	{
		widget_->Remove(); 
		
		Mouse.GetEventAnyButton().RemoveListener(widget_.Get(), &WInterfaceRoot::OnMouseButton);
		Mouse.GetEventMove().RemoveListener(widget_.Get(), &WInterfaceRoot::OnMouseMove);

		KT_LOG(KT_LOG_COMPILE_TIME_LEVEL, "Object", "Main window widget removed");
	}
}

void UInterface::UpdateFocusedWidgets()
{
	WidgetSet newFocusedWidgets{};

	if (widget_)
	{
		widget_->PopulateFocusTree(newFocusedWidgets, Mouse.GetCursorPosition());
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
