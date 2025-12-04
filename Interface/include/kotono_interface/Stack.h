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

	DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) const override;

protected:
	StackSettings stackSettings_;

	void DisplayInternal(DisplaySettings displaySettings) override;
};

