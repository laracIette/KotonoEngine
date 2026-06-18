#pragma once
#include "generated/Spacer.generated.h"
#include "Widget.h"
/// Fills the entirety of the available parent space
class WSpacer final : public WWidget
{
	GENERATED_WSPACER()

public:
	WSpacer(const EAxis axis);

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

private:
	EAxis axis_;
};

