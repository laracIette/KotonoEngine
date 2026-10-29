#include "CoreWidgets/Image/Image.h"

#include <RenderGraph/InterfaceRenderGraph.h>

WImage::WImage()
	: path_{}
{}

void WImage::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph, UColor const& tint) const
{	
	interfaceRenderGraph.drawDatas.push_back(
		UInterfaceDrawData::MakeImage(
			GetScissor(),
			GetModelMatrix(),
			tint * Tint,
			path_,
			GetIsVisible()
		)
	);
}

#include "Image.generated.inl"
