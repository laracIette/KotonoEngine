#pragma once
#include "ChildOwner/ChildOwner.h"

#include "Offset.generated.h"

/// Offset the position of the child widget
class WOffset final : public WChildOwner
{
	GENERATED()

protected:
	auto GetChildDisplaySettings(UWidgetDisplaySettings displaySettings) const -> UWidgetDisplaySettings override;

private:
	StateProperty(glm::vec2, offset_, Offset);
};

