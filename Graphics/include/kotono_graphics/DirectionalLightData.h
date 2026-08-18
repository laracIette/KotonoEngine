#pragma once
#include <glm/ext/vector_float3.hpp>
#include <kotono_common/types.h>
struct UDirectionalLightData final
{
	glm::vec3 direction;
	glm::vec3 color;
	f32 intensity;
	b8 castShadow;
};