#include "Column.h"
#include <algorithm>
#include <kotono_common/enum_utils.h>
#include <glm/common.hpp>

WColumn::WColumn(f32 spacing)
	: spacing_{ spacing }
{
}

glm::vec2 WColumn::GetContentSize(glm::vec2 bounds) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			const auto childSize{ child->GetContentSize(bounds) };
			size.x = std::max(size.x, childSize.x);
			size.y += childSize.y;
		}
	}

	if (GetValidChildrenCount() > 1)
	{
		size.y += spacing_ * static_cast<f32>(GetValidChildrenCount() - 1);
	}

	bounds = glm::min(bounds, size);
	return bounds;
}

glm::vec2 WColumn::GetDesiredSize(glm::vec2 const& bounds) const
{
	glm::vec2 size{};

	for (auto const& child : GetChildren())
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
		size.y += spacing_ * static_cast<f32>(GetValidChildrenCount() - 1);
	}
	
	return size;
}

EExpand WColumn::GetExpand() const
{
	return EExpand::Vertical;
}

EFlex WColumn::GetFlex() const
{
	return EFlex::All;
}

void WColumn::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	// Get non-expand height
	f32 nonExpandHeight{ 0.0f };
	for (const auto& child : GetChildren())
	{
		// Check if not vertical expand
		if (child && !has_flag(child->GetExpand(), EExpand::Vertical))
		{
			const auto childSize{ child->GetContentSize(displaySettings.bounds) };
			nonExpandHeight += childSize.y;
		}
	}

	// Get expand height
	f32 expandHeight{ displaySettings.bounds.y - nonExpandHeight };
	if (!GetChildren().empty())
	{
		expandHeight -= spacing_ * static_cast<f32>(GetChildren().size() - 1);
	}
	if (size const expandCount{ GetExpandCount() })
	{
		expandHeight /= static_cast<f32>(expandCount);
	}

	++displaySettings.layer;

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto settings{ displaySettings };

			if (has_flag(child->GetExpand(), EExpand::Vertical))
			{
				settings.bounds.y = expandHeight;
			}

			child->Display(settings);
			const auto childSize{ child->GetContentSize(settings.bounds) };

			displaySettings.position.y += childSize.y;
			displaySettings.position.y += spacing_;

			displaySettings.bounds.y -= childSize.y;
			displaySettings.bounds.y -= spacing_;
		}
	}
}

size WColumn::GetExpandCount() const
{
	return std::ranges::count_if(GetChildren(),
		[](WidgetPtr const& child) { return child && has_flag(child->GetExpand(), EExpand::Vertical); }
	);
}

#include "generated/Column.generated.inl"
