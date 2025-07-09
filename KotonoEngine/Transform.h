#pragma once
#include <kotono_framework/glm_includes.h>
struct UTransform final
{
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::quat rotation{ glm::identity<glm::quat>() };
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
};

