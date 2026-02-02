#include "Vertex3D.h"

bool KtVertex3D::operator==(const KtVertex3D& v) const
{
    return position == v.position
        && color == v.color
        && texCoord == v.texCoord;
}

::size std::hash<KtVertex3D>::operator()(const KtVertex3D& v) const noexcept
{
    ::size h{ 0 };
    h = combine(h, std::hash<glm::vec3>{}(v.position));
    h = combine(h, std::hash<glm::vec3>{}(v.color));
    h = combine(h, std::hash<glm::vec2>{}(v.texCoord));
    return h;
}
