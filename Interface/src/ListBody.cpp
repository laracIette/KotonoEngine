#include "ListBody.h"
#include <glm/common.hpp>

UWidgetDisplaySettings WListBody::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.bounds.y = INFINITY;

	glm::vec2 size{ 0.0f, 0.0f };

	for (auto& child : children_)
	{
		if (child)
		{
			const auto childSettings{ child->GetContentDisplaySettings(displaySettings) };
			size.x = std::max(size.x, childSettings.bounds.x);
			size.y += childSettings.bounds.y;
		}
	}

	if (!children_.empty())
	{
		size.y += spacing_ * static_cast<float>(children_.size() - 1);
	}

	displaySettings.bounds = glm::min(size, displaySettings.bounds);
	return displaySettings;
}

glm::vec2 WListBody::GetDesiredSize(const glm::vec2& bounds) const
{
	glm::vec2 size{};

	for (auto& child : children_)
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
		size.y += spacing_ * static_cast<float>(GetValidChildrenCount() - 1);
	}

	return size;
}

EFlex WListBody::GetFlex() const
{
	return EFlex::Vertical;
}

void WListBody::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	for (auto& child : children_)
	{
		if (child)
		{
			child->Display(displaySettings);

			displaySettings.position.y += child->GetContentDisplaySettings(displaySettings).bounds.y;
			displaySettings.position.y += spacing_;
		}
	}
}

#include "generated/ListBody.generated.inl"
