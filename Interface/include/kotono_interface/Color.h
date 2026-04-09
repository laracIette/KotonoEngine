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
	void Cleanup() override;
	
public:
	const UColor& GetColor() const;

	void SetColor(const UColor& color);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	UColor color_;

	UInterfaceProxy* colorProxy_;
};

