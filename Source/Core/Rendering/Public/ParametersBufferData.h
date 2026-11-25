#pragma once

#include "Containers/Array.h"
#include "types.h"

#include <glm/ext/vector_float4.hpp>

struct UParametersBufferData
{
    UArray<f32, 16>         scalars;
    UArray<glm::vec4, 16>   vectors;
    UArray<u32, 16>         textures;
};
