#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include "hash_utils.h"
struct KtVertex3D final
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;

    bool operator==(const KtVertex3D& other) const
    {
        return position == other.position
            && color == other.color
            && texCoord == other.texCoord;
    }
};

template<> 
struct std::hash<KtVertex3D>
{
    size_t operator()(const KtVertex3D& v) const noexcept
    {
        size_t h{ 0 };
        combine(h, std::hash<glm::vec3>{}(v.position));
        combine(h, std::hash<glm::vec3>{}(v.color));
        combine(h, std::hash<glm::vec2>{}(v.texCoord));
        return h;
    }
};

