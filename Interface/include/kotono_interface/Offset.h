#pragma once
#include "generated/Offset.generated.h"
#include "ChildOwner.h"
/// Offset the position of the child widget
class WOffset final : public WChildOwner
{
	GENERATED_WOFFSET()

public:
	glm::vec2 GetContentSize(glm::vec2 bounds) const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(glm::vec2, offset_, Offset);
};

