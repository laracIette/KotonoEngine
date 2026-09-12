#pragma once
#include "SceneView.h"
#include "Scissor.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float4.hpp>
#include <kotono_common/FixedHeapContainer.h>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
#include <variant>
struct UInterfaceDrawData final
{
	using Texture = std::variant<UPath, USceneView>;

	UScissor scissor;

	glm::mat4 modelMatrix;

	UPath shader;
	UPath model;

	UFixedHeapContainer<f32, 16> scalars;
	UFixedHeapContainer<glm::vec4, 16> vectors;
	UFixedHeapContainer<Texture, 16> textures;

	b8 isVisible;
};