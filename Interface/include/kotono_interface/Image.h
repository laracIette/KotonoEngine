#pragma once
#include "generated/Image.generated.h"
#include <kotono_object/Widget.h>
#include <kotono_common/Path.h>
/// Display an image over the widget's bounds
class WImage final : public WWidget
{
	GENERATED_WIMAGE()

public:
	WImage(const UPath& path = "");

	void Remove() override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

private:
	StateProperty(UPath, path_, Path);
};

