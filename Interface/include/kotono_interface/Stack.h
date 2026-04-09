#pragma once
#include "generated/Stack.generated.h"
#include "ChildrenOwner.h"
#include <vector>
class WStack final : public WChildrenOwner
{
	GENERATED_WSTACK()

public:
	struct StackSettings
	{
		WidgetVector children{};
	};

	/// Display widgets on top of each other
	WStack(const StackSettings& stackSettings);

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;
	
	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

protected:
	StackSettings stackSettings_;

	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;
};

