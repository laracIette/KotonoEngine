#include "Widget.h"
#include <kotono_common/log.h>
#include "utils.h"
#include <glm/ext/matrix_transform.hpp>
#include <kotono_input/Mouse.h>
#include <kotono_math/math_utils.h>

#if defined (_DEBUG)
static constinit u32 Count{ 0 };
#endif

#define KT_LOG_IMPORTANCE_LEVEL_WIDGET ELogImportanceLevel::Medium

WWidget::WWidget() 
	: build_{}
	, parent_{}
	, displaySettings_{}
	, isDisplayed_{ false }
{
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_WIDGET, "Interface", "{0}", ++Count);
}

WWidget::~WWidget()
{
	if (build_ && build_ != Ptr())
	{
		build_->Delete();
	}
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_WIDGET, "Interface", "{0} {1}", --Count, GetName());
}

void WWidget::PostConstruct()
{
	Base::PostConstruct();

	CacheBuild();
}

WidgetPtr WWidget::Build()
{
	return Ptr();
}

void WWidget::Display(UWidgetDisplaySettings displaySettings)
{
	isDisplayed_ = true;
	SetDisplaySettings(displaySettings);

	// If build_ is not this, call Display
	if (build_ && build_ != Ptr())
	{
		build_->Display(displaySettings);
	}

	// If build_ is this, call DisplayInternal
	if (build_ == Ptr() && IsRenderable())
	{
		const auto ds{ displaySettings };
		displaySettings = GetDisplaySettings(displaySettings);
		DisplayInternal(displaySettings);
	}
}

void WWidget::Remove()
{
	isDisplayed_ = false;

	if (build_ && build_ != Ptr())
	{
		build_->Remove();
	}
}

UWidgetDisplaySettings WWidget::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	if (build_ && build_ != Ptr())
	{
		return build_->GetDisplaySettings(displaySettings);
	}

	return displaySettings;
}

EFlex WWidget::GetFlex() const
{
	if (build_ && build_ != Ptr())
	{
		return build_->GetFlex();
	}
	return EFlex::All;
}

glm::vec2 WWidget::GetDesiredSize(glm::vec2 bounds) const
{
	if (build_ && build_ != Ptr())
	{
		return build_->GetDesiredSize(bounds);
	}
	return { 0.0f, 0.0f };
}

WidgetVector WWidget::GetWidgetTree()
{
	if (build_ && build_ != Ptr())
	{
		return build_->GetWidgetTree();
	}
	return { Ptr() };
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

void WWidget::CacheBuild()
{
	build_ = Build();
	if (build_ && build_ != Ptr())
	{
		build_->SetParent(Ptr());
	}
}

void WWidget::SetState(const StateFunction& function)
{
	const bool wasDisplayed{ isDisplayed_ };
	if (wasDisplayed)
	{
		Remove();
	}

	if (function)
	{
		function();
	}

	if (wasDisplayed)
	{
		auto displaySettings{ displaySettings_ };
		if (build_ && build_ != Ptr())
		{
			displaySettings = build_->displaySettings_;
		}
		Display(displaySettings);
	}
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
	SetState({});
}

UWidgetTreeLeaf::UWidgetTreeLeaf(const WidgetPtr& widget)
	: widget_(widget)
{
}

WidgetPtr UWidgetTreeLeaf::Widget() const
{
	return widget_;
}

void UWidgetTreeLeaf::Link() const
{
}

#include "generated/Widget.generated.inl"
