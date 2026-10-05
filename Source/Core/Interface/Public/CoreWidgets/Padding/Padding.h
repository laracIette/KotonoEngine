#pragma once
#include "ChildOwner/ChildOwner.h"

#include <Padding.h>

#include "Padding.generated.h"

/// Shrink the bounds of the child widget
class WPadding final : public WChildOwner
{
	GENERATED()

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

