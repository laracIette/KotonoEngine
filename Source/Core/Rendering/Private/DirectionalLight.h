#pragma once

#include "Containers/Array.h"
#include "types.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

inline constexpr u32 NUM_DIRECTIONAL_CASCADES{ 4 };

struct UDirectionalLight final
{
	glm::vec3 direction;
	glm::vec3 color;
	f32 intensity;
	u32 castShadow;
	u32 shadowMap;
	u32 shadowSampler;
	UArray<glm::mat4, NUM_DIRECTIONAL_CASCADES> lightViewProjs;
	UArray<f32, NUM_DIRECTIONAL_CASCADES> cascadeSplits;
};
