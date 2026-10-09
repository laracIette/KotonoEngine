#include "Widget/Widget.h"

#include "Interface/Interface.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <Logging/log.h>
#include <RenderGraph/InterfaceRenderGraph.h>
#include <math_utils/math_utils.h>

static constexpr auto getUpdatedFlex(glm::vec2 const& left, glm::vec2 const& right) noexcept -> EFlex
{
	if (left.x != right.x)
	{
		if (left.y != right.y)	return EFlex::All;
		else					return EFlex::Horizontal;
	}
	return EFlex::Vertical;
}

static constexpr auto isVisible(UWidgetDisplaySettings const& displaySettings) noexcept -> b8
{
	return is_overlapping(displaySettings.position, displaySettings.bounds, displaySettings.scissor.offset, displaySettings.scissor.extent);
}

WWidget::WWidget()
	: build_{ nullptr }
	, isDirty_{ false }
	, isVisible_{ true }
	, parent_{ nullptr }
	, propagateVisibility_{ true }
	, isDisplayed_{ false }
	, isFocused_{ false }
	, expandWeight_{ 1.0f, 1.0f }
{
}

WWidget::~WWidget()
{
	if (HasBuild())
	{
		build_->Delete();
	}

	if (GetParent())
	{
		GetParent()->Disown(Ptr());
	}
}

WidgetPtr WWidget::Build()
{
	return Ptr();
}

void WWidget::Init()
{
}

void WWidget::Display(UWidgetDisplaySettings const& displaySettings)
{
	CacheBuild();

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
	else if (isVisible(displaySettings))
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

void WWidget::Disown(WidgetPtr const& widget)
{
	if (!widget)
	{
		return;
	}

	if (widget != build_)
	{
		return;
	}

	SetState([this, widget]() {
		build_ = nullptr;
		widget->SetParent(nullptr);
	});
}

auto WWidget::GetContentSize(glm::vec2 const& bounds) const -> glm::vec2
{
	if (HasBuild())
	{
		return build_->GetContentSize(bounds);
	}

	return bounds;
}

auto WWidget::GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2
{
	if (HasBuild())
	{
		return build_->GetDesiredSize(bounds);
	}
	return { 0.0f, 0.0f };
}

auto WWidget::GetExpand() const -> EExpand
{
	if (HasBuild())
	{
		return build_->GetExpand();
	}
	return EExpand::All;
}

auto WWidget::GetFlex() const -> EFlex
{
	if (HasBuild())
	{
		return build_->GetFlex();
	}
	return EFlex::All;
}

auto WWidget::GetClassPath() const -> std::string
{
	if (parent_)
	{
		return std::format("{0} {1}", parent_->GetClassPath(), GetTypeName());
	}
	return GetTypeName();
}

UInterface* WWidget::GetInterface() const
{
	Check(Abort, parent_, "parent_ is null!");
	return parent_->GetInterface();
}

void WWidget::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	if (HasBuild() && build_->GetIsDisplayed() && GetCanPopulateRenderGraph())
	{
		build_->PopulateRenderGraph(interfaceRenderGraph);
	}
}

void WWidget::PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const
{
	if (GetIsPointHovering(cursorPosition))
	{
		widgets.Add(Ptr());
	}

	if (HasBuild())
	{
		build_->PopulateFocusTree(widgets, cursorPosition);
	}
}

auto WWidget::OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8
{
	if (!HasBuild() || !build_->GetIsDisplayed())
	{
		return INPUT_UNHANDLED;
	}

	if (build_->GetIsFocused())
	{
		return build_->OnMouseButton(button, inputState, modifier);
	}

	return INPUT_UNHANDLED;
}

auto WWidget::OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) -> b8
{
	KT_LOG(ELogImportance::Medium, "Object", "overlapping {0:30} | {1:100} | | position: {2:30} | size: {3:30} | | slot | position: {4:30} | bounds: {5:30}", GetName(), GetClassPath(), glm::to_string(GetPosition()), glm::to_string(GetSize()), glm::to_string(slotDisplaySettings_.position), glm::to_string(slotDisplaySettings_.bounds));

	if (!HasBuild() || !build_->GetIsDisplayed())
	{
		return INPUT_UNHANDLED;
	}

	if (build_->GetIsPointHovering(position))
	{
		return build_->OnMouseMove(delta, position);
	}

	return INPUT_UNHANDLED;
}

auto WWidget::OnMouseScroll(glm::vec2 const& delta) -> b8
{
	if (!HasBuild() || !build_->GetIsDisplayed())
	{
		return INPUT_UNHANDLED;
	}

	if (build_->GetIsFocused())
	{
		return build_->OnMouseScroll(delta);
	}

	return INPUT_UNHANDLED;
}

auto WWidget::OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) -> b8
{
	if (!HasBuild() || !build_->GetIsDisplayed())
	{
		return INPUT_UNHANDLED;
	}

	if (build_->GetIsFocused())
	{
		return build_->OnKeyboardKey(key, inputState, modifier);
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

void WWidget::Refresh()
{
	if (GetShouldRefresh())
	{
		isDirty_ = false;

		if (isVisible_)
		{
			Display(slotDisplaySettings_);
		}
	}
	else if (HasBuild())
	{
		build_->Refresh();
	}
}

void WWidget::CacheBuild()
{
	if (!build_)
	{
		build_ = Build();
		if (HasBuild())
		{
			build_->SetParent(Ptr());
		}
		MarkDirty();
		Init();
	}

	if (HasBuild())
	{
		build_->CacheBuild();
	}
}

auto WWidget::GetIsPointHovering(glm::vec2 const& position) const -> b8
{
	return is_point_in_rect(position, GetScissor().offset, GetScissor().extent)
		&& is_point_in_rect(position, GetPosition(), GetContentSize());
}

void WWidget::SetState(StateFunction const& function)
{
	if (isDisplayed_)
	{
		Remove();
	}

	if (function)
	{
		function();
	}

	MarkDirty();
}

void WWidget::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
}

auto WWidget::GetCanCache() const -> b8
{
	return isVisible_.GetIsValue();
}

auto WWidget::GetShouldRefresh() const -> b8
{
	return isDirty_ || !GetCanCache();
}

auto WWidget::GetCanPopulateRenderGraph() const -> b8
{
	return GetIsVisible() || !GetPropagateVisibility();
}

bool WWidget::HasBuild() const
{
	return build_ && build_ != Ptr();
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

auto WWidget::TranslationMatrix() const -> glm::mat4
{
	glm::vec2 const bounds{ GetInterface()->GetBounds() };
	return glm::translate(glm::identity<glm::mat4>(), { px_to_ndc_pos(GetPosition() + GetSize() / 2.0f, bounds), 0.0f });
}

auto WWidget::RotationMatrix() -> glm::mat4
{
	return glm::rotate(glm::identity<glm::mat4>(), 0.0f, -WorldForwardVector);
}

auto WWidget::ScaleMatrix() const -> glm::mat4
{
	glm::vec2 const bounds{ GetInterface()->GetBounds() };
	return glm::scale(glm::identity<glm::mat4>(), { px_to_ndc_size(GetSize(), bounds), 1.0f });
}

auto WWidget::ModelMatrix() const -> glm::mat4
{
	return TranslationMatrix() * RotationMatrix() * ScaleMatrix();
}

#include "Widget.generated.inl"
