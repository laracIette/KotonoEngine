#pragma once
#include <glm/vec2.hpp>
struct UScissor final
{
	glm::ivec2 offset;
	glm::uvec2 extent;

	constexpr bool operator==(const UScissor& other) const = default;
};

