#include "Column.h"
#include <algorithm>
#include <kotono_common/enum_utils.h>
#include <glm/common.hpp>

WColumn::WColumn(const f32 spacing)
	: spacing_{ spacing }
{
}

UWidgetDisplaySettings WColumn::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
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

	if (GetValidChildrenCount() > 1)
	{
		size.y += spacing_ * static_cast<float>(GetValidChildrenCount() - 1);
	}

	displaySettings.bounds = glm::min(displaySettings.bounds, size);
	return displaySettings;
}

glm::vec2 WColumn::GetDesiredSize(const glm::vec2& bounds) const
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
	float nonExpandHeight{ 0.0f };
	for (const auto& child : children_)
	{
		// Check if not vertical expand
		if (child && !has_flag(child->GetExpand(), EExpand::Vertical))
		{
			nonExpandHeight += child->GetContentDisplaySettings(displaySettings).bounds.y;
		}
	}

	// Get expand height
	float expandHeight{ displaySettings.bounds.y - nonExpandHeight };
	if (!children_.empty())
	{
		expandHeight -= spacing_ * static_cast<float>(children_.size() - 1);
	}
	if (const size expandCount{ GetExpandCount() })
	{
		expandHeight /= static_cast<float>(expandCount);
	}

	++displaySettings.layer;

	for (auto& child : children_)
	{
		if (child)
		{
			auto settings{ displaySettings };

			if (has_flag(child->GetExpand(), EExpand::Vertical))
			{
				settings.bounds.y = expandHeight;
			}

			child->Display(settings);
			const auto childSettings{ child->GetContentDisplaySettings(settings) };

			displaySettings.position.y += childSettings.bounds.y;
			displaySettings.position.y += spacing_;

			displaySettings.bounds.y -= childSettings.bounds.y;
			displaySettings.bounds.y -= spacing_;
		}
	}
}

size WColumn::GetExpandCount() const
{
	return std::count_if(children_.begin(), children_.end(),
		[](const WidgetPtr& child) { return child && has_flag(child->GetExpand(), EExpand::Vertical); }
	);
}

#include "generated/Column.generated.inl"
