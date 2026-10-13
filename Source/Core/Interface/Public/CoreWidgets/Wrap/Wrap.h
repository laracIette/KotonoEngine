#pragma once
#include "ChildOwner/ChildOwner.h"

#include "Wrap.generated.h"

/// Fills the entirety of the available parent space
class WWrap final : public WChildOwner
{
	GENERATED()

public:
	WWrap();

	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;

	auto GetExpand() const -> EExpand override;
	auto GetFlex() const -> EFlex override;
	
protected:
	auto GetChildDisplaySettings(UWidgetDisplaySettings displaySettings) const -> UWidgetDisplaySettings override;

private:
	WritableProperty(EAxis, axis_, Axis);
};