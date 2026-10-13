#include "ListBody.h"

#include <glm/common.hpp>

WListBody::WListBody() 
	: spacing_{ 0.0f }
{
}

void WListBody::Display(UWidgetDisplaySettings displaySettings)
{
	Base::Display(displaySettings);
	
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

auto WListBody::GetContentSize(glm::vec2 const& bounds) const -> glm::vec2
{
	glm::vec2 newBounds{ bounds.x, INFINITY };

	glm::vec2 size{ 0.0f, 0.0f };

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto const childSize{ child->GetContentSize(newBounds) };
			size.x = std::max(size.x, childSize.x);
			size.y += childSize.y;
		}
	}

	if (!GetChildren().empty())
	{
		size.y += spacing_ * static_cast<f32>(GetChildren().size() - 1);
	}

	size = glm::min(size, newBounds);
	return size;
}

auto WListBody::GetDesiredSize(const glm::vec2& bounds) const -> glm::vec2
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

auto WListBody::GetFlex() const -> EFlex
{
	return EFlex::Vertical;
}

#include "ListBody.generated.inl"
