#include "Stack.h"

#include <glm/common.hpp>

glm::vec2 WStack::GetContentSize(glm::vec2 bounds) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto const childSize{ child->GetContentSize(bounds) };
			size = glm::max(size, childSize);
		}
	}
	size = glm::min(size, bounds);
	return size;
}

glm::vec2 WStack::GetDesiredSize(glm::vec2 const& bounds) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto const childDesiredSize{ child->GetDesiredSize(bounds) };
			size = glm::max(size, childDesiredSize);
		}
	}

	return size;
}

EExpand WStack::GetExpand() const
{
	return EExpand::All;
}

EFlex WStack::GetFlex() const
{
	return EFlex::All;
}

void WStack::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	for (auto const& child : GetChildren())
	{
		if (child)
		{
			child->Display(displaySettings);
		}
	}
}

#include "generated/Stack.generated.inl"
