#pragma once
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_uint2.hpp>
struct UScissor final
{
	glm::ivec2 offset;
	glm::uvec2 extent;

	constexpr bool operator==(UScissor const &) const = default;
};
