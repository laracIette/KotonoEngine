#pragma once
#include "ChildOwner/ChildOwner.h"

#include "Expanded.generated.h"

/// Fills the entirety of the available parent space
class WExpanded final : public WChildOwner
{
	GENERATED()

public:
	glm::vec2 GetContentSize(glm::vec2 const& bounds) const override;
	
	EFlex GetFlex() const override;
};

