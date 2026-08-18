#include "Padding.h"

WPadding::WPadding(const UPadding& padding)
	: padding_{ padding }
{
}

UWidgetDisplaySettings WPadding::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.bounds.x -= padding_.l;
	displaySettings.bounds.x -= padding_.r;
	displaySettings.bounds.y -= padding_.t;
	displaySettings.bounds.y -= padding_.b;

	displaySettings.position.x += padding_.l;
	displaySettings.position.y += padding_.t;
	
	return displaySettings;
}

glm::vec2 WPadding::GetDesiredSize(const glm::vec2& bounds) const
{
	if (GetChild())
	{
		auto childDesiredSize{ GetChild()->GetDesiredSize(bounds) };
		childDesiredSize.x += padding_.l + padding_.r;
		childDesiredSize.y += padding_.t + padding_.b;
		return childDesiredSize;
	}

	return { 0.0f, 0.0f };
}

#include "generated/Padding.generated.inl"
