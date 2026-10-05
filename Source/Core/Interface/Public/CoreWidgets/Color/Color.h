#pragma once
#include <Widget/Widget.h>

#include <Color.h>

#include "Color.generated.h"

/// Fill the widget's bounds with a color
class WColor final : public WWidget
{
	GENERATED()

public:
	WColor();

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

	auto GetColor() const -> UColor;
	void SetColor(UBindable<UColor> const& color);

protected:
	auto GetCanCache() const -> b8 override;

private:
	UBindable<UColor> color_;
};

