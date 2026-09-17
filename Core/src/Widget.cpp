#include "Widget.h"

#include "Interface.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <kotono_common/enum_utils.h>
#include <kotono_common/log.h>
#include <kotono_graphics/InterfaceRenderGraph.h>
#include <kotono_math/math_utils.h>

static EFlex getUpdatedFlex(glm::vec2 const& left, glm::vec2 const& right) noexcept
{
	if (left.x != right.x)
	{
		if (left.y != right.y)	return EFlex::All;
		else					return EFlex::Horizontal;
	}
	return EFlex::Vertical;
}

WWidget::WWidget() 
	: build_{}
	, isDirty_{ true }
	, parent_{ nullptr }
	, canCache_{ true }
	, isVisible_{ true }
	, slotDisplaySettings_{}
	, contentSize_{}
	, isDisplayed_{ false }
{
}

WWidget::~WWidget()
{
	if (HasBuild())
	{
		build_->Delete();
	}
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

void WWidget::Display(UWidgetDisplaySettings const& displaySettings)
{
	isDisplayed_ = true;

	slotDisplaySettings_ = displaySettings;

	contentSize_ = GetContentSize(displaySettings.bounds);

	modelMatrix_ = ModelMatrix();

	// If build_ is not this, call Display
	if (HasBuild())
	{
		build_->Display(displaySettings);
	}
	// If build_ is this, call DisplayInternal
	else if (IsVisible(displaySettings))
	{
		DisplayInternal(displaySettings);
	}
}

void WWidget::Remove()
{
	isDisplayed_ = false;

	if (HasBuild())
	{
		build_->Remove();
	}
}

glm::vec2 WWidget::GetContentSize(glm::vec2 bounds) const
{
	if (HasBuild())
	{
		return build_->GetContentSize(bounds);
	}

	return bounds;
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

glm::vec2 WWidget::GetDesiredSize(glm::vec2 const& bounds) const
{
	if (HasBuild())
	{
		return build_->GetDesiredSize(bounds);
	}
	return { 0.0f, 0.0f };
}

std::string WWidget::GetClassPath() const
{
	if (parent_)
	{
		return std::format("{0} {1}", parent_->GetClassPath(), TypeName());
	}
	return TypeName();
}

UInterface* WWidget::GetInterface() const
{
	assert(parent_);
	return parent_->GetInterface();
}

void WWidget::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	if (HasBuild())
	{
		build_->PopulateRenderGraph(interfaceRenderGraph);
	}
}

void WWidget::PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const
{
	if (is_point_in_rect(cursorPosition, GetScissor().offset, GetScissor().extent)
	 && is_point_in_rect(cursorPosition, GetPosition(), GetContentSize()))
	{
		widgets.Add(Ptr());
	}

	if (HasBuild())
	{
		build_->PopulateFocusTree(widgets, cursorPosition);
	}
}

b8 WWidget::OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position)
{
	if (!HasBuild() || !build_->GetIsDisplayed())
	{
		return INPUT_UNHANDLED;
	}

	if (is_point_in_rect(position, build_->GetPosition(), build_->GetSize()))
	{
		return build_->OnMouseButton(button, inputState, position);
	}

	return INPUT_UNHANDLED;
}

b8 WWidget::OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position)
{
	KT_LOG(ELogImportanceLevel::Medium, "Interface", "overlapping {0:30} | {1:100} | | position: {2:30} | size: {3:30} | | slot | position: {4:30} | bounds: {5:30}", GetName(), GetClassPath(), glm::to_string(GetPosition()), glm::to_string(GetSize()), glm::to_string(slotDisplaySettings_.position), glm::to_string(slotDisplaySettings_.bounds));
	
	if (!HasBuild() || !build_->GetIsDisplayed())
	{
		return INPUT_UNHANDLED;
	}

	if (is_point_in_rect(position, build_->GetPosition(), build_->GetSize()))
	{
		return build_->OnMouseMove(delta, position);
	}

	return INPUT_UNHANDLED;
}

b8 WWidget::OnMouseScroll(glm::vec2 const& delta, glm::vec2 const& position)
{
	if (!HasBuild() || !build_->GetIsDisplayed())
	{
		return INPUT_UNHANDLED;
	}

	if (is_point_in_rect(position, build_->GetPosition(), build_->GetSize()))
	{
		return build_->OnMouseScroll(delta, position);
	}

	return INPUT_UNHANDLED;
}

void WWidget::OnFocused()
{
	isFocused_ = true;
}

void WWidget::OnUnfocused()
{
	isFocused_ = false;
}

b8 WWidget::GetShouldRefresh() const
{
	return isDirty_ || !canCache_;
}

void WWidget::Refresh()
{
	isDirty_ = false;

	if (isDisplayed_)
	{
		Remove();
	}

	if (isVisible_)
	{
		Display(slotDisplaySettings_);
	}
}

void WWidget::SetState(StateFunction const& function)
{
	if (function)
	{
		function();
	}

	MarkDirty();
}

void WWidget::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
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

bool WWidget::IsVisible(UWidgetDisplaySettings const& displaySettings) const
{
	return is_overlapping(displaySettings.position, displaySettings.bounds, displaySettings.scissor.offset, displaySettings.scissor.extent);
}

void WWidget::MarkDirty()
{
	isDirty_ = true;

	if (GetFlex() == EFlex::None)
	{
		return;
	}

	if (parent_)
	{
		parent_->MarkDirty();
	}
}

glm::mat4 WWidget::TranslationMatrix() const
{
	glm::vec2 const bounds{ GetInterface()->GetBounds() };
	return glm::translate(glm::identity<glm::mat4>(), { px_to_ndc_pos(GetPosition() + GetSize() / 2.0f, bounds), 0.0f });
}

glm::mat4 WWidget::RotationMatrix() const
{
	return glm::rotate(glm::identity<glm::mat4>(), 0.0f, -WorldForwardVector);
}

glm::mat4 WWidget::ScaleMatrix() const
{
	glm::vec2 const bounds{ GetInterface()->GetBounds() };
	return glm::scale(glm::identity<glm::mat4>(), { px_to_ndc_size(GetSize(), bounds), 1.0f });
}

glm::mat4 WWidget::ModelMatrix() const
{
	return TranslationMatrix() * RotationMatrix() * ScaleMatrix();
}

UWidgetTreeLeaf::UWidgetTreeLeaf(const WidgetPtr& widget)
	: widget_{ widget }
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
