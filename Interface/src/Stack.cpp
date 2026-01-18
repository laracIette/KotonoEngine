#include "Stack.h"
#include <glm/common.hpp>

WStack::WStack(const StackSettings& stackSettings) :
	WChildrenOwnerWidget(stackSettings.children),
	stackSettings_(stackSettings)
{
}

void WStack::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	for (auto* child : stackSettings_.children)
	{
		if (child)
		{
			++displaySettings.layer;
			child->Display(displaySettings);
		}
	}
}

UWidgetDisplaySettings WStack::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	glm::vec2 bounds{ 0.0f, 0.0f };

	for (auto* child : stackSettings_.children)
	{
		if (child)
		{
			const auto childSettings{ child->GetDisplaySettings(displaySettings) };
			bounds = glm::max(bounds, childSettings.bounds);
		}
	}
	displaySettings.bounds = glm::min(bounds, displaySettings.bounds);
	return displaySettings;
}

glm::vec2 WStack::GetDesiredSize() const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto* child : stackSettings_.children)
	{
		if (child)
		{
			const auto childDesiredSize{ child->GetDesiredSize() };
			size = glm::max(size, childDesiredSize);
		}
	}

	return size;
}
