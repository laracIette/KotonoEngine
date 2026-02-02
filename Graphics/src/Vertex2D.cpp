#include "Vertex2D.h"

bool KtVertex2D::operator==(const KtVertex2D& other) const
{
    return position == other.position
        && color == other.color
        && texCoord == other.texCoord;
}

size std::hash<KtVertex2D>::operator()(const KtVertex2D& v) const noexcept
{
    ::size h{ 0 };
    h = combine(h, std::hash<glm::vec2>{}(v.position));
    h = combine(h, std::hash<glm::vec3>{}(v.color));
    h = combine(h, std::hash<glm::vec2>{}(v.texCoord));
    return h;
}
