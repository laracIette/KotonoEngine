#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float4.hpp>
#include <kotono_common/FixedHeapContainer.h>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
struct USceneDrawData final
{
	f32 sortKey;

	glm::mat4 modelMatrix;
	glm::mat4 normalMatrix;

	UPath shader;
	UPath material;
	UPath model;

	UFixedHeapContainer<f32, 16> scalars;
	UFixedHeapContainer<glm::vec4, 16> vectors;
	UFixedHeapContainer<UPath, 16> textures;

	b8 isVisible;
};