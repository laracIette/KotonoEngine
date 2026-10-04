#pragma once
#include <glm/ext/vector_float2.hpp>
#include <kotono_graphics/Scissor.h>
struct UWidgetDisplaySettings final
{
	glm::vec2 position;
	glm::vec2 bounds;
	UScissor scissor;
};
