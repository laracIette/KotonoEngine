#pragma once
#include "generated/Padding.generated.h"
#include "ChildOwner/ChildOwner.h"

#include <Padding.h>

/// Shrink the bounds of the child widget
class WPadding final : public WChildOwner
{
	GENERATED_WPADDING()

public:
	WPadding(UPadding const& padding = UPadding::Zero());

public:
	glm::vec2 GetContentSize(glm::vec2 const& bounds) const override;
	glm::vec2 GetDesiredSize(glm::vec2 const& bounds) const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(UPadding, padding_, Padding, Value);
};

