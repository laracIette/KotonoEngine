#include "Box.h"
#include <glm/common.hpp>

UWidgetDisplaySettings WBox::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.bounds = glm::min(size_, displaySettings.bounds);
	return displaySettings;
}

EFlex WBox::GetFlex() const
{
	return EFlex::None;
}

glm::vec2 WBox::GetDesiredSize(glm::vec2 bounds) const
{
	return size_;
}

const glm::vec2& WBox::GetSize() const
{
	return size_;
}

void WBox::SetSize(const glm::vec2& size)
{
	size_ = size;
}

#include "generated/Box.generated.inl"
