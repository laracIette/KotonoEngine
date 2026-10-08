#include "CoreWidgets/Color/Color.h"

#include <RenderGraph/InterfaceRenderGraph.h>

WColor::WColor()
	: color_{ Colors::White }
{
}

void WColor::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	interfaceRenderGraph.drawDatas.push_back({
		.scissor{ GetScissor() },
		.modelMatrix{ GetModelMatrix() },
		.shader{ "${ENGINE_DIRECTORY}/Assets/shaders/shader2D.kasset" },
		.model{ "${ENGINE_DIRECTORY}/Assets/models/rectangle.obj" },
		.scalars{},
		.vectors{ GetColor() },
		.textures{ UPath{ "${ENGINE_DIRECTORY}/Assets/textures/white_texture.jpg" } },
		.isVisible{ GetIsVisible() },
	});
}

auto WColor::GetColor() const -> UColor
{
	return color_;
}

void WColor::SetColor(UBindable<UColor> const& color)
{
	color_ = color;
}

auto WColor::GetCanCache() const -> b8
{
	return Base::GetCanCache()
		&& color_.GetIsValue();
}

#include "Color.generated.inl"
