#include "Center.h"

WCenter::WCenter(const EAxis axis)
	: axis_{ axis }
{
}

auto WCenter::GetContentSize(glm::vec2 const& bounds) const -> glm::vec2
{
	auto size{ Base::GetContentSize(bounds) };

	switch (axis_)
	{
	case EAxis::Horizontal:
		size.x = bounds.x;
		break;
	case EAxis::Vertical:
		size.y = bounds.y;
		break;
	case EAxis::All:
		size = bounds;
		break;
	}

	return size;
}

auto WCenter::GetFlex() const -> EFlex
{
	switch (axis_)
	{
	case EAxis::Horizontal:
		return EFlex::Horizontal;
	case EAxis::Vertical:
		return EFlex::Vertical;
	default:
		return EFlex::All;
	}
}

auto WCenter::GetExpand() const -> EExpand
{
	switch (axis_)
	{
	case EAxis::Horizontal:
		return EExpand::Horizontal;
	case EAxis::Vertical:
		return EExpand::Vertical;
	default:
		return EExpand::All;
	}
}

void WCenter::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (GetChild())
	{
		const auto childSize{ GetChild()->GetContentSize(displaySettings.bounds) };

		switch (axis_)
		{
		case EAxis::Horizontal:
			displaySettings.position.x = displaySettings.position.x + (displaySettings.bounds.x - childSize.x) / 2.0f;
			break;
		case EAxis::Vertical:
			displaySettings.position.y = displaySettings.position.y + (displaySettings.bounds.y - childSize.y) / 2.0f;
			break;
		case EAxis::All:
			displaySettings.position = displaySettings.position + (displaySettings.bounds - childSize) / 2.0f;
			break;
		}
	}
	
	Base::DisplayInternal(displaySettings);
}

#include "generated/Center.generated.inl"
