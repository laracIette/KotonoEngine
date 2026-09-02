#include "ListBody.h"

#include <glm/common.hpp>

glm::vec2 WListBody::GetContentSize(glm::vec2 bounds) const
{
	bounds.y = INFINITY;

	glm::vec2 size{ 0.0f, 0.0f };

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto const childSize{ child->GetContentSize(bounds) };
			size.x = std::max(size.x, childSize.x);
			size.y += childSize.y;
		}
	}

	if (!GetChildren().empty())
	{
		size.y += spacing_ * static_cast<f32>(GetChildren().size() - 1);
	}

	bounds = glm::min(size, bounds);
	return bounds;
}

glm::vec2 WListBody::GetDesiredSize(const glm::vec2& bounds) const
{
	glm::vec2 size{};

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto const childDesiredSize{ child->GetDesiredSize(bounds) };
			size.x = std::max(size.x, childDesiredSize.x);
			size.y += childDesiredSize.y;
		}
	}

	if (GetValidChildrenCount() > 1)
	{
		size.y += spacing_ * static_cast<f32>(GetValidChildrenCount() - 1);
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

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			child->Display(displaySettings);
			auto const childSize{ child->GetContentSize(displaySettings.bounds) };

			displaySettings.position.y += childSize.y;
			displaySettings.position.y += spacing_;
		}
	}
}

#include "generated/ListBody.generated.inl"
