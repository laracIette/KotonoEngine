#pragma once
#include "generated/Wrap.generated.h"
#include "ChildOwner.h"
/// Fills the entirety of the available parent space
class WWrap final : public WChildOwner
{
	GENERATED_WWRAP()

public:
	WWrap(EAxis axis = EAxis::All);

public:
	glm::vec2 GetContentSize(glm::vec2 bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	WritableProperty(EAxis, axis_, Axis);
};