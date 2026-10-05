#pragma once
#include <glm/ext/matrix_float4x4.hpp>
struct UTransformBufferData
{
    glm::mat4 modelMatrix;
    glm::mat4 normalMatrix;
};
