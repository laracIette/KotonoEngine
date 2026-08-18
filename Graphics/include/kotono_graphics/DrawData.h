#pragma once
#include "Scissor.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float4.hpp>
#include <kotono_common/Handle.h>
#include <kotono_common/types.h>
#include <vector>
struct UDrawData final
{
	UScissor scissor;
	f32 sortKey;

	glm::mat4 modelMatrix;
	glm::mat4 normalMatrix;

	UPath shader;
	UPath material;
	UPath model;

	std::vector<f32>       scalars;  // max 16
	std::vector<glm::vec4> vectors;  // max 16
	std::vector<EHandle>   textures; // max 16

	bool isVisible;
};