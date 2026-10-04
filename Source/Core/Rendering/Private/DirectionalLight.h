#pragma once
#include <array>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <kotono_common/types.h>
inline constexpr u32 NUM_DIRECTIONAL_CASCADES{ 4 };
struct UDirectionalLight final
{
	glm::vec3 direction;
	glm::vec3 color;
	f32 intensity;
	u32 castShadow;
	u32 shadowMap;
	u32 shadowSampler;
	std::array<glm::mat4, NUM_DIRECTIONAL_CASCADES> lightViewProjs;
	std::array<f32, NUM_DIRECTIONAL_CASCADES> cascadeSplits;
};
