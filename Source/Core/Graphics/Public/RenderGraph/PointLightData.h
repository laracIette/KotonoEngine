#pragma once
#include <glm/ext/vector_float3.hpp>
#include <types.h>
struct UPointLightData final
{
	glm::vec3 position;
	f32 range;
	glm::vec3 color;
	f32 intensity;
};
