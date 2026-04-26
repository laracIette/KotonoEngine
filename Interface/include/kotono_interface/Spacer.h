#pragma once
#include "generated/Spacer.generated.h"
#include "Widget.h"
/// Fills the entirety of the available parent space
class WSpacer final : public WWidget
{
	GENERATED_WSPACER()

public:
	WSpacer(const EFlex flex);

	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EFlex GetFlex() const override;

private:
	EFlex flex_;
};

