#include "CoreWidgets/Box/Box.h"

#include <glm/common.hpp>

WBox::WBox()
	: size_{ 64.0f, 64.0f }
{
}

glm::vec2 WBox::GetContentSize(glm::vec2 const& bounds) const
{
	return glm::min(size_, bounds);
}

glm::vec2 WBox::GetDesiredSize(glm::vec2 const& bounds) const
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

auto WBox::GetChildDisplaySettings(UWidgetDisplaySettings displaySettings) const -> UWidgetDisplaySettings
{		
	displaySettings.bounds = glm::min(size_, displaySettings.bounds);
	
	return displaySettings;
}

#include "Box.generated.inl"
