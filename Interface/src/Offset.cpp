#include "Offset.h"

UWidgetDisplaySettings WOffset::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.position += offset_;
	
	if (child_)
	{
		return child_->GetContentDisplaySettings(displaySettings);
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

#include "generated/Offset.generated.inl"
