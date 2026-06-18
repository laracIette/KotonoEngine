#include "Box.h"
#include <glm/common.hpp>

UWidgetDisplaySettings WBox::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.bounds = glm::min(size_, displaySettings.bounds);
	return displaySettings;
}

glm::vec2 WBox::GetDesiredSize(const glm::vec2& bounds) const
{
	return size_;
}

EExpand WBox::GetExpand() const
{
	return EExpand::None;
}

EFlex WBox::GetFlex() const
{
	return EFlex::None;
}

#include "generated/Box.generated.inl"
