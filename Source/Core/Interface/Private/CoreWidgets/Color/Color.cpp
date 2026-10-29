#include "CoreWidgets/Color/Color.h"

#include <RenderGraph/InterfaceRenderGraph.h>

WColor::WColor()
	: color_{ Colors::White }
{
}

void WColor::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph, UColor const& tint) const
{	
	interfaceRenderGraph.drawDatas.push_back(
		UInterfaceDrawData::MakeRoundedImage(
			GetScissor(),
			GetModelMatrix(),
			GetColor() * tint * Tint,
			UPath{ "${ENGINE_DIRECTORY}/Assets/textures/white_texture.jpg" },
			{ 10.0f, 10.0f, 10.0f, 10.0f },
			GetPosition(),
			GetSize(),
			GetIsVisible()
		)
	);
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
