#include "Scrollable.h"
#include "widgets.h"
#include <kotono_common/enum_utils.h>
#include <kotono_input/Mouse.h>
#include <kotono_math/math_utils.h>
#include <glm/common.hpp>

WScrollable::WScrollable(const EAxis axis)
	: axis_{ axis }
{
}

void WScrollable::Display(UWidgetDisplaySettings const& displaySettings)
{
	Base::Display(displaySettings);

	Mouse.EventScroll().AddListener(this, &WScrollable::Scroll);
}

void WScrollable::Remove()
{
	Base::Remove();

	Mouse.EventScroll().RemoveListener(this, &WScrollable::Scroll);
}

UWidgetDisplaySettings WScrollable::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.scissor.offset = displaySettings.position;
	displaySettings.scissor.extent = displaySettings.bounds;

	displaySettings.position += offset_;

	if (GetChild())
	{
		return GetChild()->GetContentDisplaySettings(displaySettings);
	}
	return displaySettings;
}

void WScrollable::Scroll(const glm::vec2 delta)
{
	if (!IsMouseHovering())
	{
		return;
	}

	SetState([this, delta]() {
		if (!GetParent())
		{
			return;
		}

		const auto parentSize{ GetParent()->GetSize() };
		const auto maxOffset{ glm::min(parentSize - GetDesiredSize(parentSize), 0.0f) };
		
		offset_ += delta * 10.0f;
		offset_ = glm::clamp(offset_, maxOffset, { 0.0f, 0.0f });
		offset_ = {
			has_flag(axis_, EAxis::Horizontal) ? offset_.x : 0.0f,
			has_flag(axis_, EAxis::Vertical) ? offset_.y : 0.0f
		};
	});
}

#include "generated/Scrollable.generated.inl"
