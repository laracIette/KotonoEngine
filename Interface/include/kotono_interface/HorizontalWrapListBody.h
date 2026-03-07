#pragma once
#include "ChildrenOwnerWidget.h"
class WHorizontalWrapListBody : public WChildrenOwnerWidget 
{
public:
	struct HorizontalWrapListBodySettings
	{
		/// default = 0.0f
		float itemSpacing{ 0.0f };
		/// default = 0.0f
		float rowSpacing{ 0.0f };
		WidgetVector children{};
	};

	/// Defines a vertical container for widgets
	WHorizontalWrapListBody(const HorizontalWrapListBodySettings& horizontalWrapListBodySettings);

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	std::vector<glm::vec2> GetRowDisplaySizes(const UWidgetDisplaySettings& displaySettings) const;
	std::vector<glm::vec2> GetRowDesiredSizes(const glm::vec2& bounds) const;

private:
	HorizontalWrapListBodySettings horizontalWrapListBodySettings_;
};