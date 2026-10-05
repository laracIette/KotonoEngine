#pragma once
#include "ChildOwner/ChildOwner.h"

#include "Offset.generated.h"

/// Offset the position of the child widget
class WOffset final : public WChildOwner
{
	GENERATED()

public:
	glm::vec2 GetContentSize(glm::vec2 const& bounds) const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(glm::vec2, offset_, Offset);
};

