#pragma once
#include <optional>
struct KtQueueFamilyIndices
{
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t> PresentFamily;
	const bool IsComplete() const noexcept
	{
		return GraphicsFamily.has_value() && PresentFamily.has_value();
	}
};