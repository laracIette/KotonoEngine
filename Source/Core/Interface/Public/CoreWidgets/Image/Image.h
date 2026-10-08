#pragma once
#include <Widget/Widget.h>

#include "Image.generated.h"

/// Display an image over the widget's bounds
class WImage final : public WWidget
{
	GENERATED()

public:
	WImage();

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

private:
	WritableProperty(UPath, path_, Path);
};

