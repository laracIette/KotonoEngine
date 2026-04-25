#pragma once
#include "generated/Stack.generated.h"
#include "ChildrenOwner.h"
#include <vector>
/// Display widgets on top of each other
class WStack final : public WChildrenOwner
{
	GENERATED_WSTACK()

public:
	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;
	
	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;
};

