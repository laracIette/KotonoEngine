#include "Wrap.h"
#include <glm/common.hpp>
#include <kotono_common/enum_utils.h>

WWrap::WWrap(EAxis axis)
	: axis_{ axis }
{
}

glm::vec2 WWrap::GetContentSize(glm::vec2 bounds) const
{
	if (GetChild())
	{
		auto const childDesiredSize{ GetChild()->GetDesiredSize(bounds) };
		if (has_flag(axis_, EAxis::Horizontal))
		{
			bounds.x = std::min(bounds.x, childDesiredSize.x);
		}
		if (has_flag(axis_, EAxis::Vertical))
		{
			bounds.y = std::min(bounds.y, childDesiredSize.y);
		}
	}

	return Base::GetContentSize(bounds);
}

EExpand WWrap::GetExpand() const
{
	switch (axis_)
	{
	case EAxis::Horizontal:	return EExpand::Vertical;
	case EAxis::Vertical:	return EExpand::Horizontal;
	default:				return EExpand::None;
	}
}

EFlex WWrap::GetFlex() const
{
	return EFlex::All;
}

void WWrap::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (GetChild())
	{
		auto const childDesiredSize{ GetChild()->GetDesiredSize(displaySettings.bounds) };
		if (has_flag(axis_, EAxis::Horizontal))
		{
			displaySettings.bounds.x = std::min(displaySettings.bounds.x, childDesiredSize.x);
		}
		if (has_flag(axis_, EAxis::Vertical))
		{
			displaySettings.bounds.y = std::min(displaySettings.bounds.y, childDesiredSize.y);
		}
	}

	Base::DisplayInternal(displaySettings);
}

#include "generated/Wrap.generated.inl"
