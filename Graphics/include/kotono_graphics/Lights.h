#pragma once
#include <glm/vec3.hpp>
#include <kotono_common/types.h>
struct UDirectionalLight
{
	glm::vec3 direction;
	glm::vec3 color;
	f32 intensity;
};
struct UPointLight
{
	glm::vec3 position;
	f32 range;
	glm::vec3 color;
	f32 intensity;
};