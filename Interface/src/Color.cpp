#include "Color.h"

#include <glm/ext/matrix_transform.hpp>
#include <kotono_graphics/InterfaceRenderGraph.h>
#include <kotono_object/Interface.h>

WColor::WColor(const UColor& color)
	: color_{ color }
{
}

void WColor::Remove()
{
	Base::Remove();
}

void WColor::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	interfaceRenderGraph.drawDatas.push_back({
		.scissor = GetScissor(),
		.sortKey = static_cast<f32>(GetLayer()),
		.modelMatrix = ModelMatrix(),
		.normalMatrix = glm::identity<glm::mat4>(),
		.shader = "${ENGINE_DIRECTORY}/Graphics/assets/shaders/shader2D.kasset",
		.material = {},
		.model = "${ENGINE_DIRECTORY}/Graphics/assets/models/rectangle.obj",
		.scalars = {},
		.vectors = { color_ },
		.textures = { "${ENGINE_DIRECTORY}/Graphics/assets/textures/white_texture.jpg" },
		.isVisible = true,
	});
}

#include "generated/Color.generated.inl"
