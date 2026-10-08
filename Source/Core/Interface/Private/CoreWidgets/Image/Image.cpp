#include "CoreWidgets/Image/Image.h"

#include <Color.h>
#include <RenderGraph/InterfaceRenderGraph.h>

WImage::WImage()
	: path_{}
{}

void WImage::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	interfaceRenderGraph.drawDatas.push_back({
		.scissor{ GetScissor() },
		.modelMatrix{ GetModelMatrix() },
		.shader{ "${ENGINE_DIRECTORY}/Assets/shaders/shader2D.kasset" },
		.model{ "${ENGINE_DIRECTORY}/Assets/models/rectangle.obj" },
		.scalars{},
		.vectors{ Colors::White },
		.textures{ path_ },
		.isVisible{ GetIsVisible() },
	});
}

#include "Image.generated.inl"
