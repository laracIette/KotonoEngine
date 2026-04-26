#pragma once
#include "generated/Center.generated.h"
#include "ChildOwner.h"
/// Center the child widget on an Axis
class WCenter final : public WChildOwner
{
	GENERATED_WCENTER()

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

public:
	EAxis GetAxis() const;

	void SetAxis(const EAxis axis);

private:
	EAxis axis_;
};

