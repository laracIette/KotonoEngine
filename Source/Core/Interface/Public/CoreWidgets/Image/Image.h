#pragma once
#include "Image.generated.h"
#include <Widget/Widget.h>

/// Display an image over the widget's bounds
class WImage final : public WWidget
{
	GENERATED_WIMAGE()

public:
	WImage(UPath const& path = "${ENGINE_DIRECTORY}/Assets/textures/default_texture.jpg");

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

private:
	WritableProperty(UPath, path_, Path);
};

