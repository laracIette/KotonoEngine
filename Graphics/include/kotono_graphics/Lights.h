#pragma once
#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <kotono_common/types.h>
inline constexpr u32 NUM_DIRECTIONAL_CASCADES{ 4 };
struct UDirectionalLight
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
struct UPointLight
{
	glm::vec3 position;
	f32 range;
	glm::vec3 color;
	f32 intensity;
};