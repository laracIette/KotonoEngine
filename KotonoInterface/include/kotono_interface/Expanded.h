#pragma once
#include "ChildOwnerWidget.h"
class WExpanded : public WChildOwnerWidget
{
public:
	struct ExpandedSettings
	{
		WWidget* child{ nullptr };
	};

	WExpanded(const ExpandedSettings& expandedSettings);

	void Display(DisplaySettings displaySettings) override;

	DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) const override;

private:
	ExpandedSettings expandedSettings_;
};

