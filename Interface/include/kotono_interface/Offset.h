#pragma once
#include "generated/Offset.generated.h"
#include "ChildOwner.h"
/// Offset the position of the child widget
class WOffset final : public WChildOwner
{
	GENERATED_WOFFSET()

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

private:
	StateProperty(glm::vec2, offset_, Offset);
};

