#pragma once
#include "Color.h"
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
	
	static auto MakeImage(UScissor const& scissor, glm::mat4 const& modelMatrix, UColor const& color, UPath const& texture, b8 isVisible) noexcept -> UInterfaceDrawData
	{
		return {
			.scissor{ scissor },
			.modelMatrix{ modelMatrix },
			.shader{ "${ENGINE_DIRECTORY}/Assets/shaders/shader2D.kasset" },
			.model{ "${ENGINE_DIRECTORY}/Assets/models/rectangle.obj" },
			.scalars{},
			.vectors{ color },
			.textures{ texture },
			.isVisible{ isVisible }
		};
	}
	
	static auto MakeRoundedImage(UScissor const& scissor, glm::mat4 const& modelMatrix, UColor const& color, UPath const& texture, glm::vec4 const& corners, glm::vec2 const& position, glm::vec2 const& size, b8 isVisible) noexcept -> UInterfaceDrawData
	{
		return {
			.scissor{ scissor },
			.modelMatrix{ modelMatrix },
			.shader{ "${ENGINE_DIRECTORY}/Assets/shaders/rounded2D.kasset" },
			.model{ "${ENGINE_DIRECTORY}/Assets/models/rectangle.obj" },
			.scalars{},
			.vectors{ color, corners, glm::vec4{ position, size } },
			.textures{ texture },
			.isVisible{ isVisible }
		};
	}
	
	static auto MakeSceneTexture(UScissor const& scissor, glm::mat4 const& modelMatrix, UColor const& color, SceneRenderData&& sceneRenderData, b8 isVisible) noexcept -> UInterfaceDrawData
	{
		return {
			.scissor{ scissor },
			.modelMatrix{ modelMatrix },
			.shader{ "${ENGINE_DIRECTORY}/Assets/shaders/shader2D.kasset" },
			.model{ "${ENGINE_DIRECTORY}/Assets/models/rectangle.obj" },
			.scalars{},
			.vectors{ color },
			.textures{ std::move(sceneRenderData) },
			.isVisible{ isVisible }
		};
	}
};
