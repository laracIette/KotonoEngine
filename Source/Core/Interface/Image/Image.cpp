#include "Image.h"

#include <kotono_graphics/Color.h>
#include <kotono_graphics/InterfaceRenderGraph.h>

WImage::WImage(UPath const& path)
	: path_{ path }
{}

void WImage::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	interfaceRenderGraph.drawDatas.push_back({
		.scissor = GetScissor(),
		.modelMatrix = GetModelMatrix(),
		.shader = "${ENGINE_DIRECTORY}/Graphics/assets/shaders/shader2D.kasset",
		.model = "${ENGINE_DIRECTORY}/Graphics/assets/models/rectangle.obj",
		.scalars = {},
		.vectors = { Colors::White },
		.textures = { path_ },
		.isVisible = GetIsVisible(),
	});
}

#include "generated/Image.generated.inl"
