#include "ListBody.h"
#include "List.h"
#include <glm/common.hpp>

WListBody::WListBody(const ListBodySettings& listBodySettings) :
	WChildrenOwnerWidget(listBodySettings.children),
	listBodySettings_(listBodySettings)
{
}

void WListBody::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (parent_)
	{
		const auto* asList{ static_cast<WList*>(parent_) };
		displaySettings.scissor.offset = asList->displaySettings_.position;
		displaySettings.scissor.extent = asList->displaySettings_.bounds;
	}

	for (auto* child : listBodySettings_.children)
	{
		if (child)
		{
			child->Display(displaySettings);

			displaySettings.position.y += child->GetDisplaySettings(displaySettings).bounds.y;
			displaySettings.position.y += listBodySettings_.spacing;
		}
	}
}

UWidgetDisplaySettings WListBody::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.bounds.y = INFINITY;

	glm::vec2 size{ 0.0f, 0.0f };

	for (auto* child : listBodySettings_.children)
	{
		if (child)
		{
			const auto childSettings{ child->GetDisplaySettings(displaySettings) };
			size.x = std::max(size.x, childSettings.bounds.x);
			size.y += childSettings.bounds.y;
		}
	}

	if (!listBodySettings_.children.empty())
	{
		size.y += listBodySettings_.spacing * static_cast<float>(listBodySettings_.children.size() - 1);
	}

	displaySettings.bounds = glm::min(size, displaySettings.bounds);
	return displaySettings;
}

glm::vec2 WListBody::GetDesiredSize(glm::vec2 bounds) const
{
	glm::vec2 size{};

	for (auto* child : listBodySettings_.children)
	{
		if (child)
		{
			const auto childDesiredSize{ child->GetDesiredSize(bounds) };
			size.x = std::max(size.x, childDesiredSize.x);
			size.y += childDesiredSize.y;
		}
	}

	if (GetValidChildrenCount() > 1)
	{
		size.y += listBodySettings_.spacing * static_cast<float>(GetValidChildrenCount() - 1);
	}

	return size;
}
