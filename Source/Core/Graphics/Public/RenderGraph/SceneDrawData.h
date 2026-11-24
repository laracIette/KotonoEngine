#pragma once

#include "Containers/HeapArray.h"
#include "Path/Path.h"
#include "types.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float4.hpp>

struct USceneDrawData final
{
	f32 sortKey;

	glm::mat4 modelMatrix;
	glm::mat4 normalMatrix;

	UPath shader;
	UPath material;
	UPath model;

	UHeapArray<f32, 16> scalars;
	UHeapArray<glm::vec4, 16> vectors;
	UHeapArray<UPath, 16> textures;

	b8 isVisible;
};
