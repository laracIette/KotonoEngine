#pragma once
#include "ChildOwnerWidget.h"
class WOffset : public WChildOwnerWidget
{
public:
	struct OffsetSettings
	{
		/// default = { 0.0f, 0.0f }
		glm::vec2 size{ 0.0f, 0.0f };
		WWidget* child{ nullptr };
	};

	/// Offset the position of the child widget
	WOffset(const OffsetSettings& offsetSettings);

	void Display(DisplaySettings displaySettings) override;

	DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) const override;

protected:
	OffsetSettings offsetSettings_;
};

