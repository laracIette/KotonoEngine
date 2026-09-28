#include "Scrollable.h"

#include <glm/common.hpp>
#include <kotono_common/enum_utils.h>

WScrollable::WScrollable()
	: axis_{ EAxis::All }
{
}

b8 WScrollable::OnMouseScroll(glm::vec2 const& delta)
{
	auto const bounds{ GetSize() };
	auto const desiredSize{ GetDesiredSize(bounds) };
	auto const maxOffset{ glm::min(bounds - desiredSize, 0.0f) };

	auto offset{ offset_ };
		
	offset += delta * 10.0f;
	offset = glm::clamp(offset, maxOffset, { 0.0f, 0.0f });
	offset = {
		has_flag(axis_, EAxis::Horizontal) ? offset.x : 0.0f,
		has_flag(axis_, EAxis::Vertical) ? offset.y : 0.0f
	};

	if (offset == offset_)
	{
		return INPUT_UNHANDLED;
	}

	SetState([this, offset]() {
		offset_ = offset;
	});

	return INPUT_HANDLED;
}

void WScrollable::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
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

#include "generated/Scrollable.generated.inl"
