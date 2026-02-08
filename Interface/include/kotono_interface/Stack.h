#pragma once
#include "ChildrenOwnerWidget.h"
#include <vector>
class WStack : public WChildrenOwnerWidget
{
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

