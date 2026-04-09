#include "Scrollable.h"
#include "widgets.h"
#include <kotono_common/bitwise_utils.h>
#include <kotono_input/Mouse.h>

WScrollable::WScrollable(const ScrollableSettings& scrollableSettings)
	: scrollableSettings_(scrollableSettings)
	, offset_(0.0f)
{
	assert(scrollableSettings_.axis != EAxis::All);
}

WidgetPtr WScrollable::Build()
{
	GetScrollEvent().AddListener(this, &WScrollable::Scroll);

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
	GetScrollEvent().RemoveListener(this, &WScrollable::Scroll);

	Base::Cleanup();
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

UEvent<float>& WScrollable::GetScrollEvent() const
{
	if (scrollableSettings_.axis == EAxis::Horizontal)
	{
		return Mouse.EventHorizontalScroll();
	}
	else
	{
		return Mouse.EventVerticalScroll();
	}
}

#include "generated/Scrollable.generated.inl"
