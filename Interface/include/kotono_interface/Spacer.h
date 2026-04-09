#pragma once
#include "generated/Spacer.generated.h"
#include "Widget.h"
/// Fills the entirety of the available parent space
class WSpacer final : public WWidget
{
	GENERATED_WSPACER()

public:
	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EFlex GetFlex() const override;
};

