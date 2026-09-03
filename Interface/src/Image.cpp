#include "Image.h"

#include <glm/ext/matrix_transform.hpp>
#include <kotono_graphics/Color.h>
#include <kotono_graphics/InterfaceRenderGraph.h>

WImage::WImage(UPath const& path)
	: path_{ path }
{}

void WImage::Remove()
{
	Base::Remove();
}

void WImage::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
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
		.vectors = { Colors::White },
		.textures = { path_ },
		.isVisible = true,
	});
}

#include "generated/Image.generated.inl"
