#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <kotono_common/types.h>
struct UDirectionalLight
{
	glm::vec3 direction;
	glm::vec3 color;
	f32 intensity;
	u32 castShadow;
	u32 shadowMap;
	u32 shadowSampler;
	glm::mat4 lightViewProj;
};
struct UPointLight
{
	glm::vec3 position;
	f32 range;
	glm::vec3 color;
	f32 intensity;
};