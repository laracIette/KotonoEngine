#pragma once
#include "generated/Expanded.generated.h"
#include "ChildOwner.h"
/// Fills the entirety of the available parent space
class WExpanded final : public WChildOwner
{
	GENERATED_WEXPANDED()

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;
	
	EFlex GetFlex() const override;
};

