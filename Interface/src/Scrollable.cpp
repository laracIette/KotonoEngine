#include "Scrollable.h"
#include "widgets.h"
#include <kotono_common/bitwise_utils.h>
#include <kotono_input/Mouse.h>

WScrollable::WScrollable(const ScrollableSettings& scrollableSettings)
	: scrollableSettings_(scrollableSettings)
	, offset_(0.0f)
{
}

WidgetPtr WScrollable::Build()
{
	switch (scrollableSettings_.axis)
	{
	case EAxis::Horizontal:
		Mouse.EventHorizontalScroll().AddListener(this, &WScrollable::Scroll);
		break;
	case EAxis::Vertical:
		Mouse.EventVerticalScroll().AddListener(this, &WScrollable::Scroll);
		break;
	default:
		break;
	}

	const glm::vec2 offset(
		has_flag(scrollableSettings_.axis, EAxis::Horizontal) ? offset_ : 0.0f,
		has_flag(scrollableSettings_.axis, EAxis::Vertical) ? offset_ : 0.0f
	);

	return new WOffset({
		.offset = offset,
		.child = scrollableSettings_.child,
	});
}

void WScrollable::Cleanup()
{
	switch (scrollableSettings_.axis)
	{
	case EAxis::Horizontal:
		Mouse.EventHorizontalScroll().RemoveListener(this, &WScrollable::Scroll);
		break;
	case EAxis::Vertical:
		Mouse.EventVerticalScroll().RemoveListener(this, &WScrollable::Scroll);
		break;
	default:
		break;
	}

	WWidget::Cleanup();
}

void WScrollable::Scroll(const float delta)
{
	if (!IsMouseHovering())
	{
		return;
	}

	SetState([this, delta]()
		{
			const auto maxOffset{ std::min(Size().y - GetDesiredSize(Size()).y, 0.0f) };
			offset_ += delta * 10.0f;
			offset_ = std::clamp(offset_, maxOffset, 0.0f);
		}
	);
}
