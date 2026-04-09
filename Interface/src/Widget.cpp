#include "Widget.h"
#include <kotono_common/log.h>
#include "utils.h"
#include <glm/ext/matrix_transform.hpp>
#include <kotono_input/Mouse.h>
#include <kotono_math/math_utils.h>

WWidget::WWidget() 
	: cachedBuild_([this]() { return Build(); })
{
}

void WWidget::CacheBuild()
{
	cachedBuild_.TryUpdateValue();
	WidgetPtr build{ cachedBuild_.Value() };
	if (build && build != Ptr())
	{
		build->CacheBuild();
	}
}

WidgetPtr WWidget::Build()
{
	return Ptr();
}

void WWidget::Cleanup()
{
	WidgetPtr build{ cachedBuild_.Value() };
	if (build && build != Ptr())
	{
		build->Cleanup();
		build->Delete();
	}
}

void WWidget::Display(UWidgetDisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);

	WidgetPtr build{ cachedBuild_.Value() };
	if (build && build != Ptr())
	{
		build->Display(displaySettings);
	}
	else if (IsRenderable())
	{
		displaySettings = GetDisplaySettings(displaySettings);
		DisplayInternal(displaySettings);
	}
}

UWidgetDisplaySettings WWidget::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	const WidgetPtr build{ cachedBuild_.Value() };
	if (build && build != Ptr())
	{
		return build->GetDisplaySettings(displaySettings);
	}

	return displaySettings;
}

EFlex WWidget::GetFlex() const
{
	const WidgetPtr build{ cachedBuild_.Value() };
	if (build && build != Ptr())
	{
		return build->GetFlex();
	}
	return EFlex::All;
}

glm::vec2 WWidget::GetDesiredSize(glm::vec2 bounds) const
{
	const WidgetPtr build{ cachedBuild_.Value() };
	if (build && build != Ptr())
	{
		return build->GetDesiredSize(bounds);
	}
	return { 0.0f, 0.0f };
}

WidgetVector WWidget::GetWidgetTree()
{
	WidgetPtr build{ cachedBuild_.Value() };
	if (build && build != Ptr())
	{
		return { build };
	}
	return { Ptr() };
}

void WWidget::Rebuild()
{
	auto displaySettings{ displaySettings_ };
	const WidgetPtr build{ cachedBuild_.Value() };
	if (build && build != Ptr())
	{
		displaySettings = build->displaySettings_;
	}
	Cleanup();
	CacheBuild();
	Display(displaySettings);
}

glm::vec2 WWidget::Position() const
{
	return displaySettings_.position;
}

glm::vec2 WWidget::Size() const
{
	return displaySettings_.bounds;
}

i32 WWidget::Layer() const
{
	return displaySettings_.layer;
}

bool WWidget::IsRenderable() const
{
	return is_overlapping(Position(), Size(), displaySettings_.scissor.offset, displaySettings_.scissor.extent);
}

bool WWidget::IsMouseHovering() const
{
	return is_point_in_rect(Mouse.CursorPosition(), Position(), Size());
}

void WWidget::SetParent(WidgetPtr parent)
{
	parent_ = parent;
}

void WWidget::SetState(const StateFunction& function)
{
	function();
	Refresh();
}

void WWidget::SetDisplaySettings(const UWidgetDisplaySettings& displaySettings)
{
	displaySettings_ = displaySettings;
}

glm::mat4 WWidget::TranslationMatrix() const
{
	return glm::translate(glm::identity<glm::mat4>(), { px_to_ndc_pos(displaySettings_.position + displaySettings_.bounds / 2.0f), 0.0f });
}

glm::mat4 WWidget::RotationMatrix() const
{
	return glm::rotate(glm::identity<glm::mat4>(), 0.0f, { 0.0f, 0.0f, 1.0f });
}

glm::mat4 WWidget::ScaleMatrix() const
{
	return glm::scale(glm::identity<glm::mat4>(), { px_to_ndc_size(displaySettings_.bounds), 1.0f });
}

glm::mat4 WWidget::ModelMatrix() const
{
	return TranslationMatrix() * RotationMatrix() * ScaleMatrix();
}

void WWidget::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
}

void WWidget::Refresh()
{
	cachedBuild_.MarkDirty();
	Rebuild();
}

#include "generated/Widget.generated.inl"
