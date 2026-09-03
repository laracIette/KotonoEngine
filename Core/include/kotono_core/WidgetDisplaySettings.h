#pragma once
#include <glm/ext/vector_float2.hpp>
#include <kotono_common/types.h>
#include <kotono_graphics/Scissor.h>
struct UWidgetDisplaySettings final
{
	glm::vec2 position;
	glm::vec2 bounds;
	i32 layer;
	UScissor scissor;
};
