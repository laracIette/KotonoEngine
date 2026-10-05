#pragma once
#include <array>
#include <glm/ext/vector_float4.hpp>
#include <types.h>
struct UParametersBufferData
{
    std::array<f32, 16>         scalars;
    std::array<glm::vec4, 16>   vectors;
    std::array<u32, 16>         textures;
};
