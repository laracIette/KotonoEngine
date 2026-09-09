#include "Color.h"

#include <glm/ext/matrix_transform.hpp>
#include <kotono_graphics/InterfaceRenderGraph.h>
#include <kotono_core/Interface.h>

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
		.modelMatrix = ModelMatrix(),
		.shader = "${ENGINE_DIRECTORY}/Graphics/assets/shaders/shader2D.kasset",
		.model = "${ENGINE_DIRECTORY}/Graphics/assets/models/rectangle.obj",
		.scalars = {},
		.vectors = { color_ },
		.textures = { "${ENGINE_DIRECTORY}/Graphics/assets/textures/white_texture.jpg" },
		.isVisible = GetIsVisible(),
	});
}

#include "generated/Color.generated.inl"
