#pragma once
#include "generated/Wrap.generated.h"
#include "ChildOwner.h"
/// Fills the entirety of the available parent space
class WWrap final : public WChildOwner
{
	GENERATED_WWRAP()

public:
	WWrap();

	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EFlex GetFlex() const override;

private:
	WritableProperty(EAxis, axis_, Axis);
};