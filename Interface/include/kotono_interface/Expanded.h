#pragma once
#include "generated/Expanded.generated.h"
#include "ChildOwner.h"
/// Fills the entirety of the available parent space
class WExpanded final : public WChildOwner
{
	GENERATED_WEXPANDED()

public:
	glm::vec2 GetContentSize(glm::vec2 bounds) const override;
	
	EFlex GetFlex() const override;
};

