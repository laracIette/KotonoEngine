#include "Vertex3D.h"
#include <glm/gtx/hash.hpp>
#include <kotono_common/hash_utils.h>

bool KtVertex3D::operator==(const KtVertex3D& v) const
{
    return position == v.position
        && normal == v.normal
        && uv == v.uv
        && tangent == v.tangent;
}

::size std::hash<KtVertex3D>::operator()(const KtVertex3D& v) const noexcept
{
    ::size h{ 0 };
    h = combine(h, std::hash<glm::vec3>{}(v.position));
    h = combine(h, std::hash<glm::vec3>{}(v.normal));
    h = combine(h, std::hash<glm::vec2>{}(v.uv));
    h = combine(h, std::hash<glm::vec2>{}(v.tangent));
    return h;
}
