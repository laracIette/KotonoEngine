#include "Scrollable.h"
#include "widgets.h"
#include <kotono_common/bitwise_utils.h>
#include <kotono_input/Mouse.h>
#include <glm/common.hpp>

void WScrollable::Display(UWidgetDisplaySettings displaySettings)
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
	displaySettings.position += offset_;

	if (child_)
	{
		return child_->GetContentDisplaySettings(displaySettings);
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
		const auto maxOffset{ glm::min(GetSize() - GetDesiredSize(GetSize()), 0.0f) };
		offset_ += delta * 10.0f;
		offset_ = glm::clamp(offset_, maxOffset, { 0.0f, 0.0f });
		offset_ = {
			has_flag(axis_, EAxis::Horizontal) ? offset_.x : 0.0f,
			has_flag(axis_, EAxis::Vertical) ? offset_.y : 0.0f
		};
	});
}

#include "generated/Scrollable.generated.inl"
