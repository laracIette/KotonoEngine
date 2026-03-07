#include "HorizontalWrapListBody.h"
#include "HorizontalWrapList.h"
#include <glm/common.hpp>

// todo: add spacings

WHorizontalWrapListBody::WHorizontalWrapListBody(const HorizontalWrapListBodySettings& horizontalWrapListBodySettings)
	: WChildrenOwnerWidget(horizontalWrapListBodySettings.children)
	, horizontalWrapListBodySettings_(horizontalWrapListBodySettings)
{
}

UWidgetDisplaySettings WHorizontalWrapListBody::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	displaySettings.bounds.y = INFINITY;

	auto rowSizes{ GetRowDisplaySizes(displaySettings) };

	glm::vec2 size{ 0.0f, 0.0f };
	for (const auto& rowSize : rowSizes)
	{
		size.x = std::max(size.x, rowSize.x);
		size.y += rowSize.y;
	}
	
	displaySettings.bounds = glm::min(displaySettings.bounds, size);
	return displaySettings;
}

glm::vec2 WHorizontalWrapListBody::GetDesiredSize(glm::vec2 bounds) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	const auto rowSizes{ GetRowDesiredSizes(bounds) };
	for (const auto& rowSize : rowSizes)
	{
		size.x = std::max(size.x, rowSize.x);
		size.y += rowSize.y;
	}

	return size;
}

void WHorizontalWrapListBody::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	++displaySettings.layer;

	if (parent_)
	{
		const auto* asList{ static_cast<WHorizontalWrapList*>(parent_) };
		displaySettings.scissor.offset = asList->displaySettings_.position;
		displaySettings.scissor.extent = asList->displaySettings_.bounds;
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
				displaySettings.position.y += rowSize.y;

				displaySettings.bounds.x = baseDisplaySettings.bounds.x;
				displaySettings.bounds.y -= rowSize.y; // useless because bounds.y can only be INF, but here for consistency

				rowSize = { 0.0f, 0.0f };
			}

			child->Display(displaySettings);
			const auto childSettings{ child->GetDisplaySettings(displaySettings) };

			rowSize.x += childSettings.bounds.x;
			rowSize.y = std::max(rowSize.y, childSettings.bounds.y);

			displaySettings.position.x += childSettings.bounds.x;
			displaySettings.bounds.x -= childSettings.bounds.x;
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
			if (!isRowEmpty() && childDesiredSize.x > newDisplayBounds.x)
			{
				rowSizes.push_back(rowSize);

				newDisplayBounds.x = displaySettings.bounds.x;
				newDisplayBounds.y -= rowSize.y; // useless because bounds.y can only be INF, but here for consistency

				rowSize = { 0.0f, 0.0f };
			}

			rowSize.x += childDesiredSize.x;
			rowSize.y = std::max(rowSize.y, childDesiredSize.y);

			newDisplayBounds.x -= childDesiredSize.x;
		}
	}

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
			if (!isRowEmpty() && rowSize.x + childDesiredSize.x > bounds.x)
			{
				rowSizes.push_back(rowSize);
				rowSize = { 0.0f, 0.0f };
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
