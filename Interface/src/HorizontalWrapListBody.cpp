#include "HorizontalWrapListBody.h"
#include <glm/common.hpp>

UWidgetDisplaySettings WHorizontalWrapListBody::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.bounds.y = INFINITY;

	const auto rowSizes{ GetRowDisplaySizes(displaySettings) };
	glm::vec2 size{ 0.0f, 0.0f };

	for (const auto& rowSize : rowSizes)
	{
		size.x = std::max(size.x, rowSize.x);
		size.y += rowSize.y;
	}

	if (rowSizes.size())
	{
		size.y += rowSpacing_ * static_cast<float>(rowSizes.size() - 1);
	}
	
	displaySettings.bounds = glm::min(displaySettings.bounds, size);
	return displaySettings;
}

glm::vec2 WHorizontalWrapListBody::GetDesiredSize(const glm::vec2& bounds) const
{
	const auto rowSizes{ GetRowDesiredSizes(bounds) };
	glm::vec2 size{ 0.0f, 0.0f };

	for (const auto& rowSize : rowSizes)
	{
		size.x = std::max(size.x, rowSize.x);
		size.y += rowSize.y;
	}
	
	if (rowSizes.size())
	{
		size.y += rowSpacing_ * static_cast<float>(rowSizes.size() - 1);
	}

	return size;
}

EFlex WHorizontalWrapListBody::GetFlex() const
{
	return EFlex::All;
}

void WHorizontalWrapListBody::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	const auto baseDisplaySettings{ displaySettings };

	glm::vec2 rowSize{ 0.0f, 0.0f };
	auto isRowEmpty{ [&rowSize]() { return rowSize == glm::vec2{ 0.0f, 0.0f }; } };

	for (auto& child : children_)
	{
		if (child)
		{
			const auto childDesiredSize{ child->GetDesiredSize(displaySettings.bounds) };

			if (!isRowEmpty() && childDesiredSize.x > displaySettings.bounds.x)
			{
				displaySettings.position.x = baseDisplaySettings.position.x;
				displaySettings.position.y += rowSize.y + rowSpacing_;

				displaySettings.bounds.x = baseDisplaySettings.bounds.x;
				displaySettings.bounds.y -= rowSize.y + rowSpacing_; // useless because bounds.y can only be INF, but here for consistency

				rowSize = { 0.0f, 0.0f };
			}

			child->Display(displaySettings);
			const auto childSettings{ child->GetContentDisplaySettings(displaySettings) };

			rowSize.x += childSettings.bounds.x + itemSpacing_;
			rowSize.y = std::max(rowSize.y, childSettings.bounds.y);

			displaySettings.position.x += childSettings.bounds.x + itemSpacing_;
			displaySettings.bounds.x -= childSettings.bounds.x + itemSpacing_;
		}
	}
}

std::vector<glm::vec2> WHorizontalWrapListBody::GetRowDisplaySizes(const UWidgetDisplaySettings& displaySettings) const
{
	std::vector<glm::vec2> rowSizes{};
	glm::vec2 rowSize{ 0.0f, 0.0f };

	auto isRowEmpty{ [&rowSize]() { return rowSize == glm::vec2{ 0.0f, 0.0f }; } };

	auto newDisplayBounds{ displaySettings.bounds };

	for (auto& child : children_)
	{
		if (child)
		{
			const auto childDesiredSize{ child->GetDesiredSize(newDisplayBounds) };

			if (!isRowEmpty())
			{
				const auto requiredSize{ childDesiredSize.x + itemSpacing_ };

				// Try to insert an item spacing
				if (requiredSize <= newDisplayBounds.x)
				{
					rowSize.x += itemSpacing_;
					newDisplayBounds.x -= itemSpacing_;
				}
				// Submit the row if the child doesn't fit
				else
				{
					rowSizes.push_back(rowSize);

					newDisplayBounds.x = displaySettings.bounds.x;
					newDisplayBounds.y -= rowSize.y + rowSpacing_; // useless because bounds.y can only be INF, but here for consistency

					rowSize = { 0.0f, 0.0f };
				}
			}

			rowSize.x += childDesiredSize.x;
			rowSize.y = std::max(rowSize.y, childDesiredSize.y);

			newDisplayBounds.x -= childDesiredSize.x;
		}
	}

	// Add the last computed row if necessary
	if (!isRowEmpty())
	{
		rowSizes.push_back(rowSize);
	}

	return rowSizes;
}

std::vector<glm::vec2> WHorizontalWrapListBody::GetRowDesiredSizes(const glm::vec2& bounds) const
{
	std::vector<glm::vec2> rowSizes{};
	glm::vec2 rowSize{ 0.0f, 0.0f };

	auto isRowEmpty{ [&rowSize]() { return rowSize == glm::vec2{ 0.0f, 0.0f }; } };

	for (auto& child : children_)
	{
		if (child)
		{
			const auto childDesiredSize{ child->GetDesiredSize(bounds) };

			if (!isRowEmpty())
			{
				const auto requiredSize{ childDesiredSize.x + itemSpacing_ };

				if (rowSize.x + requiredSize <= bounds.x)
				{
					rowSize.x += itemSpacing_;
				}
				else
				{
					rowSizes.push_back(rowSize);
					rowSize = { 0.0f, 0.0f };
				}
			}

			rowSize.x += childDesiredSize.x;
			rowSize.y = std::max(rowSize.y, childDesiredSize.y);
		}
	}

	if (!isRowEmpty())
	{
		rowSizes.push_back(rowSize);
	}

	return rowSizes;
}

#include "generated/HorizontalWrapListBody.generated.inl"
