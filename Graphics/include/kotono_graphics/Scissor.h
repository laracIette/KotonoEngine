#pragma once
#include <kotono_common/hash_utils.h>
#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
struct KtScissor final
{
	glm::ivec2 offset;
	glm::uvec2 extent;
};

template<>
struct std::hash<KtScissor>
{
    size_t operator()(const KtScissor& s) const noexcept
    {
        size_t h{ 0 };
        combine(h, std::hash<glm::ivec2>{}(s.offset));
        combine(h, std::hash<glm::uvec2>{}(s.extent));
        return h;
    }
};
