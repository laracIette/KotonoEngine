#include "CoreWidgets/Column/Column.h"

#include <algorithm>
#include <glm/common.hpp>
#include <enum_utils.h>

WColumn::WColumn()
	: spacing_{ 0.0f }
{
}

void WColumn::Display(UWidgetDisplaySettings displaySettings)
{	
	Base::Display(displaySettings);
	
	// Get non-expand height
	f32 nonExpandHeight{ 0.0f };
	for (const auto& child : GetChildren())
	{
		// Check if not vertical expand
		if (child && !child->GetHasExpand(EExpand::Vertical))
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
	expandHeight /= GetChildrenExpandWeight();

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto settings{ displaySettings };

			if (child->GetHasExpand(EExpand::Vertical))
			{
				settings.bounds.y = expandHeight * child->GetExpandWeight().y;
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

auto WColumn::GetContentSize(glm::vec2 const& bounds) const -> glm::vec2
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

	size = glm::min(size, bounds);
	return bounds;
}

auto WColumn::GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2
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

auto WColumn::GetExpand() const -> EExpand
{
	return std::ranges::any_of(GetChildren(),
		[](WidgetPtr const& child) { return child && child->GetHasExpand(EExpand::Horizontal); }
	) ? EExpand::All : EExpand::Vertical;
}

auto WColumn::GetFlex() const -> EFlex
{
	return EFlex::All;
}

auto WColumn::GetChildrenExpandWeight() const -> f32
{
	return std::ranges::fold_left(GetChildren() 
        | std::views::filter([](WidgetPtr const& child) { 
            return child && child->GetHasExpand(EExpand::Vertical); 
        })
        | std::views::transform([](WidgetPtr const& child) { 
            return child->GetExpandWeight().y; 
        }),
        0.0f,
        std::plus{}
    );
}

#include "Column.generated.inl"
