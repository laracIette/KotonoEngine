#pragma once
#include <Widget/Widget.h>

#include "Spacer.generated.h"

/// Fills the entirety of the available parent space
class WSpacer final : public WWidget
{
	GENERATED()

public:
	WSpacer(EAxis axis);

public:
	glm::vec2 GetContentSize(glm::vec2 const& bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

private:
	EAxis axis_;
};

