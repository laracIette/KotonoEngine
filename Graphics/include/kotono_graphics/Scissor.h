#pragma once
#include <glm/vec2.hpp>
struct KtScissor final
{
	glm::ivec2 offset;
	glm::uvec2 extent;

	constexpr bool operator==(const KtScissor& other) const = default;
};

