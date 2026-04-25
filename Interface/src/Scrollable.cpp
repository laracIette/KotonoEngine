#include "Scrollable.h"
#include "widgets.h"
#include <kotono_common/bitwise_utils.h>
#include <kotono_input/Mouse.h>

WidgetPtr WScrollable::Build()
{
	UPtr offset{ Create<WOffset>{}() };
	offset->SetChild(child_);
	offset->SetOffset({
		has_flag(axis_, EAxis::Horizontal) ? offset_ : 0.0f,
		has_flag(axis_, EAxis::Vertical) ? offset_ : 0.0f
	});

	return offset;
}

void WScrollable::Display(UWidgetDisplaySettings displaySettings)
{
	Base::Display(displaySettings);

	GetScrollEvent().AddListener(this, &WScrollable::Scroll);
}

void WScrollable::Remove()
{
	Base::Remove();

	GetScrollEvent().RemoveListener(this, &WScrollable::Scroll);
}

EAxis WScrollable::GetAxis() const
{
	return axis_;
}

void WScrollable::SetAxis(const EAxis axis)
{
	assert(axis != EAxis::All);
	axis_ = axis;
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
	if (axis_ == EAxis::Horizontal)
	{
		return Mouse.EventHorizontalScroll();
	}
	else
	{
		return Mouse.EventVerticalScroll();
	}
}

#include "generated/Scrollable.generated.inl"
