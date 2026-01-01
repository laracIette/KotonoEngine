#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <kotono_common/hash_utils.h>
#include <kotono_common/types.h>
struct KtVertex2D final
{
    glm::vec2 position;
    glm::vec3 color;
    glm::vec2 texCoord;

    bool operator==(const KtVertex2D& other) const;
};

template<> 
struct std::hash<KtVertex2D>
{
    ::size operator()(const KtVertex2D& v) const noexcept;
};
