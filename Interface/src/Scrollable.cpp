#include "Scrollable.h"

#include <glm/common.hpp>
#include <kotono_common/enum_utils.h>
#include <kotono_input/Mouse.h>

WScrollable::WScrollable(EAxis axis)
	: axis_{ axis }
{
}

void WScrollable::Display(UWidgetDisplaySettings const& displaySettings)
{
	Base::Display(displaySettings);

	Mouse.GetEventScroll().AddListener(this, &WScrollable::Scroll);
}

void WScrollable::Remove()
{
	Base::Remove();

	Mouse.GetEventScroll().RemoveListener(this, &WScrollable::Scroll);
}

glm::vec2 WScrollable::GetContentSize(glm::vec2 bounds) const
{
	return Base::GetContentSize(bounds);
}

void WScrollable::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	displaySettings.scissor.offset = displaySettings.position;
	displaySettings.scissor.extent = displaySettings.bounds;

	displaySettings.position += offset_;

	switch (axis_)
	{
	case EAxis::Horizontal:
		displaySettings.bounds.x = INFINITY;
		break;
	case EAxis::Vertical:
		displaySettings.bounds.y = INFINITY;
		break;
	case EAxis::All:
		displaySettings.bounds = { INFINITY, INFINITY };
		break;
	}

	Base::DisplayInternal(displaySettings);
}

void WScrollable::Scroll(glm::vec2 const& delta)
{
	if (!GetIsFocused())
	{
		return;
	}

	SetState([this, delta]() {
		auto const bounds{ GetSlotDisplaySettings().bounds };
		auto const desiredSize{ GetDesiredSize(bounds) };
		auto const maxOffset{ glm::min(bounds - desiredSize, 0.0f) };
		
		offset_ += delta * 10.0f;
		offset_ = glm::clamp(offset_, maxOffset, { 0.0f, 0.0f });
		offset_ = {
			has_flag(axis_, EAxis::Horizontal) ? offset_.x : 0.0f,
			has_flag(axis_, EAxis::Vertical) ? offset_.y : 0.0f
		};
	});
}

#include "generated/Scrollable.generated.inl"
