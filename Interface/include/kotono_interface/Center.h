#pragma once
#include "generated/Center.generated.h"
#include "ChildOwner.h"
/// Center the child widget on an Axis
class WCenter final : public WChildOwner
{
	GENERATED_WCENTER()

public:
	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

public:
	EAxis GetAxis() const;

	void SetAxis(const EAxis axis);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	EAxis axis_;
};

