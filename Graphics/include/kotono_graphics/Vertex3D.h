#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <kotono_common/types.h>
struct KtVertex3D final
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec4 tangent;

    bool operator==(const KtVertex3D& other) const;
};

template<> 
struct std::hash<KtVertex3D>
{
    ::size operator()(const KtVertex3D& v) const noexcept;
};
