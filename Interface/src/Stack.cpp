#include "Stack.h"
#include <glm/common.hpp>

void WStack::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	for (auto const& child : GetChildren())
	{
		if (child)
		{
			++displaySettings.layer;
			child->Display(displaySettings);
		}
	}
}

UWidgetDisplaySettings WStack::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	glm::vec2 bounds{ 0.0f, 0.0f };

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			const auto childSettings{ child->GetContentDisplaySettings(displaySettings) };
			bounds = glm::max(bounds, childSettings.bounds);
		}
	}
	displaySettings.bounds = glm::min(bounds, displaySettings.bounds);
	return displaySettings;
}

glm::vec2 WStack::GetDesiredSize(const glm::vec2& bounds) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			const auto childDesiredSize{ child->GetDesiredSize(bounds) };
			size = glm::max(size, childDesiredSize);
		}
	}

	return size;
}

#include "generated/Stack.generated.inl"
