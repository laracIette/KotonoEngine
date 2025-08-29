#include "Stack.h"

WStack::WStack(const StackSettings& stackSettings) :
	WChildrenOwnerWidget(stackSettings.children),
	stackSettings_(stackSettings)
{
}

void WStack::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);

	for (auto* child : stackSettings_.children)
	{
		if (child)
		{
			++displaySettings.layer;
			child->Display(displaySettings);
		}
	}
}

WWidget::DisplaySettings WStack::GetDisplaySettings(DisplaySettings displaySettings)
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
