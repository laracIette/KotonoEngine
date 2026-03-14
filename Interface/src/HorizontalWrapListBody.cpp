#include "HorizontalWrapListBody.h"
#include "HorizontalWrapList.h"
#include <glm/common.hpp>
#include <kotono_math/math_utils.h>

WHorizontalWrapListBody::WHorizontalWrapListBody(const HorizontalWrapListBodySettings& horizontalWrapListBodySettings)
	: WChildrenOwnerWidget(horizontalWrapListBodySettings.children)
	, horizontalWrapListBodySettings_(horizontalWrapListBodySettings)
{
}

UWidgetDisplaySettings WHorizontalWrapListBody::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
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
		size.y += horizontalWrapListBodySettings_.rowSpacing * static_cast<float>(rowSizes.size() - 1);
	}
	
	displaySettings.bounds = glm::min(displaySettings.bounds, size);
	return displaySettings;
}

glm::vec2 WHorizontalWrapListBody::GetDesiredSize(glm::vec2 bounds) const
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
		size.y += horizontalWrapListBodySettings_.rowSpacing * static_cast<float>(rowSizes.size() - 1);
	}

	return size;
}

void WHorizontalWrapListBody::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (parent_) // todo: move to scrollable ?
	{
		const auto* asList{ static_cast<WHorizontalWrapList*>(parent_) };

		glm::vec2 newScissorOffset;
		glm::vec2 newScissorExtent;
		compute_intersect(
			displaySettings_.scissor.offset,
			displaySettings_.scissor.extent,
			asList->displaySettings_.position,
			asList->displaySettings_.bounds,
			newScissorOffset,
			newScissorExtent

		);
		displaySettings.scissor.offset = newScissorOffset;
		displaySettings.scissor.extent = newScissorExtent;
	}

	const auto baseDisplaySettings{ displaySettings };

	glm::vec2 rowSize{ 0.0f, 0.0f };
	auto isRowEmpty{ [&rowSize]() { return rowSize == glm::vec2{ 0.0f, 0.0f }; } };

	for (auto* child : horizontalWrapListBodySettings_.children)
	{
		if (child)
		{
			const auto childDesiredSize{ child->GetDesiredSize(displaySettings.bounds) };

			if (!isRowEmpty() && childDesiredSize.x > displaySettings.bounds.x)
			{
				displaySettings.position.x = baseDisplaySettings.position.x;
				displaySettings.position.y += rowSize.y + horizontalWrapListBodySettings_.rowSpacing;

				displaySettings.bounds.x = baseDisplaySettings.bounds.x;
				displaySettings.bounds.y -= rowSize.y + horizontalWrapListBodySettings_.rowSpacing; // useless because bounds.y can only be INF, but here for consistency

				rowSize = { 0.0f, 0.0f };
			}

			child->Display(displaySettings);
			const auto childSettings{ child->GetDisplaySettings(displaySettings) };

			rowSize.x += childSettings.bounds.x + horizontalWrapListBodySettings_.itemSpacing;
			rowSize.y = std::max(rowSize.y, childSettings.bounds.y);

			displaySettings.position.x += childSettings.bounds.x + horizontalWrapListBodySettings_.itemSpacing;
			displaySettings.bounds.x -= childSettings.bounds.x + horizontalWrapListBodySettings_.itemSpacing;
		}
	}
}

std::vector<glm::vec2> WHorizontalWrapListBody::GetRowDisplaySizes(const UWidgetDisplaySettings& displaySettings) const
{
	std::vector<glm::vec2> rowSizes{};
	glm::vec2 rowSize{ 0.0f, 0.0f };

	auto isRowEmpty{ [&rowSize]() { return rowSize == glm::vec2{ 0.0f, 0.0f }; } };

	auto newDisplayBounds{ displaySettings.bounds };

	for (auto* child : horizontalWrapListBodySettings_.children)
	{
		if (child)
		{
			const auto childDesiredSize{ child->GetDesiredSize(newDisplayBounds) };

			if (!isRowEmpty())
			{
				const auto requiredSize{ childDesiredSize.x + horizontalWrapListBodySettings_.itemSpacing };

				// Try to insert an item spacing
				if (requiredSize <= newDisplayBounds.x)
				{
					rowSize.x += horizontalWrapListBodySettings_.itemSpacing;
					newDisplayBounds.x -= horizontalWrapListBodySettings_.itemSpacing;
				}
				// Submit the row if the child doesn't fit
				else
				{
					rowSizes.push_back(rowSize);

					newDisplayBounds.x = displaySettings.bounds.x;
					newDisplayBounds.y -= rowSize.y + horizontalWrapListBodySettings_.rowSpacing; // useless because bounds.y can only be INF, but here for consistency

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

	for (auto* child : horizontalWrapListBodySettings_.children)
	{
		if (child)
		{
			const auto childDesiredSize{ child->GetDesiredSize(bounds) };

			if (!isRowEmpty())
			{
				const auto requiredSize{ childDesiredSize.x + horizontalWrapListBodySettings_.itemSpacing };

				if (rowSize.x + requiredSize <= bounds.x)
				{
					rowSize.x += horizontalWrapListBodySettings_.itemSpacing;
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
