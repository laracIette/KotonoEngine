#pragma once
#include "ChildrenOwnerWidget.h"
#include <vector>
class WStack : public WChildrenOwnerWidget
{
public:
	struct StackSettings
	{
		std::vector<WWidget*> children{};
	};

	/// Display widgets on top of each other
	WStack(const StackSettings& stackSettings);

	void Display(DisplaySettings displaySettings) override;

	DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) override;

protected:
	StackSettings stackSettings_;
};

