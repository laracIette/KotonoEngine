#pragma once
#include <optional>
#include <kotono_common/types.h>
struct KtQueueFamilyIndices final
{
	std::optional<u32> graphicsFamily;
	std::optional<u32> presentFamily;
	constexpr bool IsComplete() const noexcept
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};