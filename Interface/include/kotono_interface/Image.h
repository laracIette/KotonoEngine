#pragma once
#include "generated/Image.generated.h"
#include <kotono_core/Widget.h>
#include <kotono_common/Path.h>
/// Display an image over the widget's bounds
class WImage final : public WWidget
{
	GENERATED_WIMAGE()

public:
	WImage(UPath const& path = "${ENGINE_DIRECTORY}/Graphics/assets/textures/default_texture.jpg");

	void Remove() override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

private:
	WritableProperty(UPath, path_, Path);
};

