#include "Color.h"

#include <kotono_graphics/InterfaceRenderGraph.h>

WColor::WColor()
	: color_{ Colors::White }
{
}

void WColor::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	interfaceRenderGraph.drawDatas.push_back({
		.scissor = GetScissor(),
		.modelMatrix = GetModelMatrix(),
		.shader = "${ENGINE_DIRECTORY}/Graphics/assets/shaders/shader2D.kasset",
		.model = "${ENGINE_DIRECTORY}/Graphics/assets/models/rectangle.obj",
		.scalars = {},
		.vectors = { GetColor() },
		.textures = { "${ENGINE_DIRECTORY}/Graphics/assets/textures/white_texture.jpg" },
		.isVisible = GetIsVisible(),
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
	return color_.GetIsValue();
}

#include "generated/Color.generated.inl"
