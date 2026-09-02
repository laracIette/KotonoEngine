#include "Padding.h"

WPadding::WPadding(UPadding const& padding)
	: padding_{ padding }
{
}

glm::vec2 WPadding::GetContentSize(glm::vec2 bounds) const
{
	bounds.x -= padding_.l;
	bounds.x -= padding_.r;
	bounds.y -= padding_.t;
	bounds.y -= padding_.b;
	
	return bounds;
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

void WPadding::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	displaySettings.bounds.x -= padding_.l;
	displaySettings.bounds.x -= padding_.r;
	displaySettings.bounds.y -= padding_.t;
	displaySettings.bounds.y -= padding_.b;

	displaySettings.position.x += padding_.l;
	displaySettings.position.y += padding_.t;

	Base::DisplayInternal(displaySettings);
}

#include "generated/Padding.generated.inl"
