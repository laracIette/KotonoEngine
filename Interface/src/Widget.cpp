#include "Widget.h"
#include <kotono_common/log.h>
#include "utils.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <kotono_input/Mouse.h>
#include <kotono_math/math_utils.h>
#include <kotono_common/enum_utils.h>
#include <kotono_platform/glm_utils.h>

#if defined (_DEBUG)
static constinit u32 Count{ 0 };
#endif

#define KT_LOG_IMPORTANCE_LEVEL_WIDGET ELogImportanceLevel::Medium

static EAxis getUpdatedAxis(const glm::vec2& left, const glm::vec2& right) noexcept;

WWidget::WWidget() 
	: build_{}
	, parent_{}
	, slotDisplaySettings_{}
	, displaySettings_{}
	, isDisplayed_{ false }
	, wasMouseHovering_{ false }
{
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_WIDGET, "Interface", "{0}", ++Count);
}

WWidget::~WWidget()
{
	if (HasBuild())
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

	slotDisplaySettings_ = displaySettings;

	// If build_ is not this, call Display
	if (HasBuild())
	{
		displaySettings_ = displaySettings;
		build_->Display(displaySettings);
	}
	// If build_ is this, call DisplayInternal
	else if (IsRenderable(displaySettings))
	{
		displaySettings = GetContentDisplaySettings(displaySettings);
		displaySettings_ = displaySettings;
		DisplayInternal(displaySettings);
	}

	Mouse.EventMove().AddListener(this, &WWidget::OnMouseMove);
}

void WWidget::Remove()
{
	isDisplayed_ = false;

	if (HasBuild())
	{
		build_->Remove();
	}

	Mouse.EventMove().RemoveListener(this, &WWidget::OnMouseMove);
}

UWidgetDisplaySettings WWidget::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	if (HasBuild())
	{
		return build_->GetContentDisplaySettings(displaySettings);
	}

	return displaySettings;
}

EExpand WWidget::GetExpand() const
{
	if (HasBuild())
	{
		return build_->GetExpand();
	}
	return EExpand::All;
}

EFlex WWidget::GetFlex() const
{
	if (HasBuild())
	{
		return build_->GetFlex();
	}
	return EFlex::All;
}

glm::vec2 WWidget::GetDesiredSize(const glm::vec2& bounds) const
{
	if (HasBuild())
	{
		return build_->GetDesiredSize(bounds);
	}
	return { 0.0f, 0.0f };
}

WidgetVector WWidget::WidgetTree() const
{
	if (HasBuild())
	{
		return build_->WidgetTree();
	}
	return { Ptr() };
}

std::string WWidget::ClassPath() const
{
	if (parent_)
	{
		return std::format("{0} {1}", parent_->ClassPath(), TypeName());
	}
	return TypeName();
}

bool WWidget::IsMouseHovering() const
{
	return is_point_in_rect(Mouse.CursorPosition(), GetPosition(), GetSize());
}

const glm::vec2& WWidget::GetPosition() const
{
	return displaySettings_.position;
}

const glm::vec2& WWidget::GetSize() const
{
	return displaySettings_.bounds;
}

u32 WWidget::GetLayer() const
{
	return displaySettings_.layer;
}

const KtScissor& WWidget::GetScissor() const
{
	return displaySettings_.scissor;
}

void WWidget::SetState(const StateFunction& function)
{
	const auto oldDesiredSize{ GetDesiredSize(glm::vec2{ INFINITY }) };

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
		const auto newDesiredSize{ GetDesiredSize(glm::vec2{ INFINITY }) };

		if (oldDesiredSize == newDesiredSize)
		{
			Display(slotDisplaySettings_);
		}
		else if (UPtr ancestor{ FindNonFlexAncestor(getUpdatedAxis(oldDesiredSize, newDesiredSize)) })
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_WIDGET, "Interface", "ancestor: {0}", ancestor->GetName());
			ancestor->Remove();
			ancestor->Display(ancestor->slotDisplaySettings_);
		}
	}
}

glm::mat4 WWidget::TranslationMatrix() const
{
	return glm::translate(glm::identity<glm::mat4>(), { px_to_ndc_pos(GetPosition() + GetSize() / 2.0f), 0.0f});
}

glm::mat4 WWidget::RotationMatrix() const
{
	return glm::rotate(glm::identity<glm::mat4>(), 0.0f, -WorldForwardVector);
}

glm::mat4 WWidget::ScaleMatrix() const
{
	return glm::scale(glm::identity<glm::mat4>(), { px_to_ndc_size(GetSize()), 1.0f });
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

void WWidget::CacheBuild()
{
	build_ = Build();
	if (HasBuild())
	{
		build_->SetParent(Ptr());
	}
}

bool WWidget::HasBuild() const
{
	return build_ && build_ != Ptr();
}

bool WWidget::IsRenderable(const UWidgetDisplaySettings& displaySettings) const
{
	return is_overlapping(displaySettings.position, displaySettings.bounds, displaySettings.scissor.offset, displaySettings.scissor.extent);
}

void WWidget::OnMouseMove(const glm::vec2 delta)
{
	if (!IsMouseHovering())
	{
		wasMouseHovering_ = false;
		return;
	}

	if (!wasMouseHovering_)
	{
		wasMouseHovering_ = true;
		KT_LOG(ELogImportanceLevel::Medium, "Interface", "overlapping {0:20} | position: {1:30}, size: {2:30}", GetName(), glm::to_string(GetPosition()), glm::to_string(GetSize()));
	}
}

UPtr<WWidget> WWidget::FindNonFlexAncestor(const EAxis axis) const
{
	if (!parent_)
	{
		return Ptr();
	}

	if (!has_flag(parent_->GetFlex(), static_cast<EFlex>(axis)))
	{
		return parent_;
	}

	return parent_->FindNonFlexAncestor(axis);
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

EAxis getUpdatedAxis(const glm::vec2& left, const glm::vec2& right) noexcept
{
	return left.x != right.x
		? left.y != right.y
			? EAxis::All
			: EAxis::Horizontal
		: EAxis::Vertical;
}

#include "generated/Widget.generated.inl"
