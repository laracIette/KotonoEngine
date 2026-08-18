#pragma once
#include "generated/Spacer.generated.h"
#include <kotono_object/Widget.h>
/// Fills the entirety of the available parent space
class WSpacer final : public WWidget
{
	GENERATED_WSPACER()

public:
	WSpacer(EAxis axis);

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

private:
	EAxis axis_;
};

