#pragma once
#include "generated/HorizontalWrapListBody.generated.h"
#include "ChildrenOwner.h"
class WHorizontalWrapListBody final : public WChildrenOwner
{
	GENERATED_WHORIZONTALWRAPLISTBODY()

public:
	glm::vec2 GetContentSize(glm::vec2 bounds) const override;
	glm::vec2 GetDesiredSize(const glm::vec2& bounds) const override;

	EFlex GetFlex() const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	std::vector<glm::vec2> GetRowDisplaySizes(glm::vec2 const& bounds) const;
	std::vector<glm::vec2> GetRowDesiredSizes(glm::vec2 const& bounds) const;

private:
	StateProperty(float, itemSpacing_, ItemSpacing);
	StateProperty(float, rowSpacing_, RowSpacing);
};