#pragma once
#include "generated/HorizontalWrapListBody.generated.h"
#include "Widget.h"
class WHorizontalWrapListBody final : public WWidget 
{
	GENERATED_WHORIZONTALWRAPLISTBODY()

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

public:
	const WidgetPool& GetChildren() const;
	float GetItemSpacing() const;
	float GetRowSpacing() const;

	void SetChildren(const WidgetPool& children);
	void SetItemSpacing(const float itemSpacing);
	void SetRowSpacing(const float rowSpacing);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	std::vector<glm::vec2> GetRowDisplaySizes(const UWidgetDisplaySettings& displaySettings) const;
	std::vector<glm::vec2> GetRowDesiredSizes(const glm::vec2& bounds) const;

private:
	WidgetPool children_;
	float itemSpacing_;
	float rowSpacing_;
};