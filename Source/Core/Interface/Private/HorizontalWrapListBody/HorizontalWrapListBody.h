#pragma once
#include "ChildrenOwner/ChildrenOwner.h"

#include "HorizontalWrapListBody.generated.h"

class WHorizontalWrapListBody final : public WChildrenOwner
{
	GENERATED()

public:
	WHorizontalWrapListBody();
	
	void Display(UWidgetDisplaySettings displaySettings) override;
	
	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;
	auto GetDesiredSize(const glm::vec2& bounds) const -> glm::vec2 override;

	auto GetFlex() const -> EFlex override;

private:
	auto GetRowDisplaySizes(glm::vec2 const& bounds) const -> std::vector<glm::vec2>;
	auto GetRowDesiredSizes(glm::vec2 const& bounds) const -> std::vector<glm::vec2>;

private:
	StateProperty(float, itemSpacing_, ItemSpacing);
	StateProperty(float, rowSpacing_, RowSpacing);
};