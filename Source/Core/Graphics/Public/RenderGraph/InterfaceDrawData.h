#pragma once
#include "SceneRenderGraph.h"
#include "SceneView.h"
#include "Scissor.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float4.hpp>
#include <Containers/FixedHeapContainer.h>
#include <Path/Path.h>
#include <types.h>
#include <variant>
struct UInterfaceDrawData final
{
	struct SceneRenderData
	{
		USceneView sceneView;
		USceneRenderGraph sceneRenderGraph;
	};

	using Texture = std::variant<UPath, SceneRenderData>;

	UScissor scissor;

	glm::mat4 modelMatrix;

	UPath shader;
	UPath model;

	UFixedHeapContainer<f32, 16> scalars;
	UFixedHeapContainer<glm::vec4, 16> vectors;
	UFixedHeapContainer<Texture, 16> textures;

	b8 isVisible;
};
