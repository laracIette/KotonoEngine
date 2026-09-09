#pragma once
#include "generated/Color.generated.h"
#include <kotono_core/Widget.h>
#include <kotono_graphics/Color.h>
/// Fill the widget's bounds with a color
class WColor final : public WWidget
{
	GENERATED_WCOLOR()

public:
	WColor(UColor const& color = Colors::White);

	void Remove() override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

private:
	WritableProperty(UColor, color_, Color);
};

