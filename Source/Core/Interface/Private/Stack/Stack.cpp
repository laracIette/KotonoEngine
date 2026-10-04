#include "Stack.h"

#include <glm/common.hpp>
#include <kotono_common/enum_utils.h>

auto WStack::GetContentSize(glm::vec2 const& bounds) const -> glm::vec2
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto const childSize{ child->GetContentSize(bounds) };
			size = glm::max(size, childSize);
		}
	}

	size = glm::min(size, bounds);
	return size;
}

auto WStack::GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto const childDesiredSize{ child->GetDesiredSize(bounds) };
			size = glm::max(size, childDesiredSize);
		}
	}

	return size;
}

auto WStack::GetExpand() const -> EExpand
{
	auto result{ EExpand::None };

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto const childExpand{ child->GetExpand() };
			if (childExpand == EExpand::All)
			{
				return EExpand::All;
			}

			result |= childExpand;
		}
	}

	return result;
}

auto WStack::GetFlex() const -> EFlex
{
	return EFlex::All;
}

void WStack::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	for (auto const& child : GetChildren())
	{
		if (child)
		{
			child->Display(displaySettings);
		}
	}
}

#include "generated/Stack.generated.inl"
