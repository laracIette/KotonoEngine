#include "Expanded.h"

glm::vec2 WExpanded::GetContentSize(glm::vec2 const& bounds) const
{
	return bounds;
}

EFlex WExpanded::GetFlex() const
{
	return EFlex::All;
}

#include "generated/Expanded.generated.inl"
