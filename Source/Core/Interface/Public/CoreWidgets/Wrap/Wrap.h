#pragma once
#include "ChildOwner/ChildOwner.h"

#include "Wrap.generated.h"

/// Fills the entirety of the available parent space
class WWrap final : public WChildOwner
{
	GENERATED()

public:
	WWrap(EAxis axis = EAxis::All);

public:
	glm::vec2 GetContentSize(glm::vec2 const& bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	WritableProperty(EAxis, axis_, Axis);
};