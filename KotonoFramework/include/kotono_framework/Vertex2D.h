#pragma once
#include "glm_includes.h"
struct KtVertex2D final
{
    glm::vec2 Position;
    glm::vec3 KtColor;
    glm::vec2 TexCoord;

    bool operator==(const KtVertex2D& other) const
    {
        return Position == other.Position
            && KtColor == other.KtColor
            && TexCoord == other.TexCoord;
    }
};

namespace std
{
    template<> 
    struct hash<KtVertex2D>
    {
        size_t operator()(KtVertex2D const& vertex) const
        {
            return ((hash<glm::vec2>{}(vertex.Position) ^
                (hash<glm::vec3>{}(vertex.KtColor) << 1)) >> 1) ^
                (hash<glm::vec2>{}(vertex.TexCoord) << 1);
        }
    };
}

