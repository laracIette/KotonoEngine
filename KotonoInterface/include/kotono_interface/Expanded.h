#pragma once
#include "ChildOwnerWidget.h"
class WExpanded : public WChildOwnerWidget
{
public:
	struct ExpandedSettings
	{
		WWidget* child{ nullptr };
	};

	/// Fills the entirety of the available parent space
	WExpanded(const ExpandedSettings& expandedSettings);

	DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) const override;
	
	EFlex GetFlex() const override;

protected:
	ExpandedSettings expandedSettings_;

	void DisplayInternal(DisplaySettings displaySettings) override;
};

