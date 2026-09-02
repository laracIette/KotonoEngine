#include "Offset.h"

glm::vec2 WOffset::GetContentSize(glm::vec2 bounds) const
{
	return Base::GetContentSize(bounds);
}

void WOffset::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	displaySettings.position += offset_;

	Base::DisplayInternal(displaySettings);
}

#include "generated/Offset.generated.inl"
