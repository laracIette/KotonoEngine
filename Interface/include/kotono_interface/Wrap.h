#pragma once
#include "generated/Wrap.generated.h"
#include "ChildOwner.h"
/// Fills the entirety of the available parent space
class WWrap final : public WChildOwner
{
	GENERATED_WWRAP()

public:
	WWrap(const EAxis axis = EAxis::All);

	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

private:
	WritableProperty(EAxis, axis_, Axis);
};