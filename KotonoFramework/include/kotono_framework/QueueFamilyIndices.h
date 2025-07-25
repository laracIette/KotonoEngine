#pragma once
#include <optional>
struct KtQueueFamilyIndices final
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	const bool IsComplete() const noexcept
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};