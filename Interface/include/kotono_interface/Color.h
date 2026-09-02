#pragma once
#include "generated/Color.generated.h"
#include <kotono_object/Widget.h>
#include <kotono_graphics/Color.h>
/// Fill the widget's bounds with a color
class WColor final : public WWidget
{
	GENERATED_WCOLOR()

public:
	WColor(const UColor& color = Colors::White);

	void Remove() override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

private:
	StateProperty(UColor, color_, Color);
};

