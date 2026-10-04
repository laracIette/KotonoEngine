#pragma once
#include <glm/ext/vector_float3.hpp>
#include <kotono_common/types.h>
struct UPointLight final
{
	glm::vec3 position;
	f32 range;
	glm::vec3 color;
	f32 intensity;
};
