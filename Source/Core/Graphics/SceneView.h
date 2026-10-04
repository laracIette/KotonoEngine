#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <kotono_common/types.h>
struct USceneView final
{
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec3 viewPos;
    glm::vec2 extent;
    f32 time;
    f32 fov;
    f32 aspectRatio;
    f32 depthNear;
};