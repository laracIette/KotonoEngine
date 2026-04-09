#include "Offset.h"

UWidgetDisplaySettings WOffset::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.position += offset_;
	
	if (child_)
	{
		return child_->GetDisplaySettings(displaySettings);
	}
	return displaySettings;
}

const glm::vec2& WOffset::GetOffset() const
{
	return offset_;
}

void WOffset::SetOffset(const glm::vec2& offset)
{
	offset_ = offset;
}

void WOffset::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (child_)
	{
		child_->Display(displaySettings);
	}
}

#include "generated/Offset.generated.inl"
