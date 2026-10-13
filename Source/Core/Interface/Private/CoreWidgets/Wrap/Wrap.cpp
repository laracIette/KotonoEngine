#include "CoreWidgets/Wrap/Wrap.h"

#include <enum_utils.h>

WWrap::WWrap()
	: axis_{ EAxis::All }
{
}

glm::vec2 WWrap::GetContentSize(glm::vec2 const& bounds) const
{
	glm::vec2 newBounds{ bounds };

	if (GetChild())
	{
		auto const childDesiredSize{ GetChild()->GetDesiredSize(bounds) };
		if (has_flag(axis_, EAxis::Horizontal))
		{
			newBounds.x = std::min(bounds.x, childDesiredSize.x);
		}
		if (has_flag(axis_, EAxis::Vertical))
		{
			newBounds.y = std::min(bounds.y, childDesiredSize.y);
		}
	}

	return Base::GetContentSize(newBounds);
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

auto WWrap::GetChildDisplaySettings(UWidgetDisplaySettings displaySettings) const -> UWidgetDisplaySettings
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
	
	return displaySettings;
}

#include "Wrap.generated.inl"
