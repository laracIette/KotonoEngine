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
    combine(h, std::hash<glm::vec3>{}(v.position));
    combine(h, std::hash<glm::vec3>{}(v.color));
    combine(h, std::hash<glm::vec2>{}(v.texCoord));
    return h;
}
