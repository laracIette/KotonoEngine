#pragma once
#include "glm_includes.h"
#include <vulkan/vulkan.h>
#include <array>
struct KtVertex
{
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoord;

    bool operator==(const KtVertex& other) const
    {
        return Position == other.Position
            && Color == other.Color
            && TexCoord == other.TexCoord;
    }
};

namespace std
{
    template<> struct hash<KtVertex>
    {
        size_t operator()(KtVertex const& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.Position) ^
                (hash<glm::vec3>()(vertex.Color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.TexCoord) << 1);
        }
    };
}

