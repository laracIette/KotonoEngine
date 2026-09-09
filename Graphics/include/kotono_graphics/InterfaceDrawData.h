#pragma once
#include "SceneView.h"
#include "Scissor.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float4.hpp>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
#include <variant>
#include <vector>
struct UInterfaceDrawData final
{
	using Texture = std::variant<UPath, USceneView>;

	UScissor scissor;

	glm::mat4 modelMatrix;

	UPath shader;
	UPath model;

	std::vector<f32>       scalars;  // max 16
	std::vector<glm::vec4> vectors;  // max 16
	std::vector<Texture>   textures; // max 16

	b8 isVisible;
};