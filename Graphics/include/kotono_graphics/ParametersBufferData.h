#pragma once
#include <array>
#include <glm/vec4.hpp>
#include <kotono_common/types.h>
struct UParametersBufferData
{
    std::array<f32, 16>         scalars;
    std::array<glm::vec4, 16>   vectors;
    std::array<u32, 16>         textures;
};
