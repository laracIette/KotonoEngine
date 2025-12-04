#pragma once
#include <optional>
struct KtQueueFamilyIndices final
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	bool IsComplete() const noexcept
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};