#pragma once
#include "ChildOwnerWidget.h"
class WOffset : public WChildOwnerWidget
{
public:
	struct OffsetSettings
	{
		/// default = { 0.0f, 0.0f }
		glm::vec2 offset{ 0.0f, 0.0f };
		WWidget* child{ nullptr };
	};

	/// Offset the position of the child widget
	WOffset(const OffsetSettings& offsetSettings);

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

protected:
	OffsetSettings offsetSettings_;

	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;
};

