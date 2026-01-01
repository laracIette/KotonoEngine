#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <kotono_common/hash_utils.h>
#include <kotono_common/types.h>
struct KtVertex3D final
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;

    bool operator==(const KtVertex3D& other) const;
};

template<> 
struct std::hash<KtVertex3D>
{
    ::size operator()(const KtVertex3D& v) const noexcept;
};
