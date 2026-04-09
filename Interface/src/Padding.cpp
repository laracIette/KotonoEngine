#include "Padding.h"

UWidgetDisplaySettings WPadding::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.bounds.x -= padding_.l;
	displaySettings.bounds.x -= padding_.r;
	displaySettings.bounds.y -= padding_.t;
	displaySettings.bounds.y -= padding_.b;

	displaySettings.position.x += padding_.l;
	displaySettings.position.y += padding_.t;
	
	return displaySettings;
}

glm::vec2 WPadding::GetDesiredSize(glm::vec2 bounds) const
{
	if (child_)
	{
		auto childDesiredSize{ child_->GetDesiredSize(bounds) };
		childDesiredSize.x += padding_.l + padding_.r;
		childDesiredSize.y += padding_.t + padding_.b;
		return childDesiredSize;
	}

	return { 0.0f, 0.0f };
}

const UPadding& WPadding::GetPadding() const
{
	return padding_;
}

void WPadding::SetPadding(const UPadding& padding)
{
	padding_ = padding;
}

void WPadding::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (child_)
	{
		child_->Display(displaySettings);
	}
}

#include "generated/Padding.generated.inl"
