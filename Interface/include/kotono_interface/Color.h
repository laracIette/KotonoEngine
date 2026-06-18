#pragma once
#include "generated/Color.generated.h"
#include "Widget.h"
#include <kotono_graphics/Color.h>
class UInterfaceProxy;
/// Fill the widget's bounds with a color
class WColor final : public WWidget
{
	GENERATED_WCOLOR()

public:
	WColor(const UColor& color = Colors::White);

public:
	void Remove() override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(UColor, color_, Color);

	UInterfaceProxy* colorProxy_;
};

