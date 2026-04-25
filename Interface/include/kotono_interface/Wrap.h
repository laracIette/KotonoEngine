#pragma once
#include "generated/Wrap.generated.h"
#include "ChildOwner.h"
/// Fills the entirety of the available parent space
class WWrap final : public WChildOwner
{
	GENERATED_WWRAP()

public:
	WWrap();

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EFlex GetFlex() const override;

public:
	EAxis GetAxis() const;

	void SetAxis(const EAxis axis);

protected:
	EAxis axis_;

	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;
};