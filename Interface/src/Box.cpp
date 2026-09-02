#include "Box.h"

#include <glm/common.hpp>

WBox::WBox(glm::vec2 const& size)
	: size_{ size }
{
}

glm::vec2 WBox::GetContentSize(glm::vec2 bounds) const
{
	bounds = glm::min(size_, bounds);

	return Base::GetContentSize(bounds);
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

void WBox::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	displaySettings.bounds = glm::min(size_, displaySettings.bounds);

	Base::DisplayInternal(displaySettings);
}

#include "generated/Box.generated.inl"
