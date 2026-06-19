#pragma once
#include "generated/HorizontalWrapListBody.generated.h"
#include "ChildrenOwner.h"
class WHorizontalWrapListBody final : public WChildrenOwner
{
	GENERATED_WHORIZONTALWRAPLISTBODY()

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;
	glm::vec2 GetDesiredSize(const glm::vec2& bounds) const override;

	EFlex GetFlex() const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	std::vector<glm::vec2> GetRowDisplaySizes(const UWidgetDisplaySettings& displaySettings) const;
	std::vector<glm::vec2> GetRowDesiredSizes(const glm::vec2& bounds) const;

private:
	StateProperty(float, itemSpacing_, ItemSpacing);
	StateProperty(float, rowSpacing_, RowSpacing);
};