#pragma once
#include "generated/Offset.generated.h"
#include "ChildOwner.h"
/// Offset the position of the child widget
class WOffset final : public WChildOwner
{
	GENERATED_WOFFSET()

public:	
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

public:	
	const glm::vec2& GetOffset() const;

	void SetOffset(const glm::vec2& offset);

private:
	glm::vec2 offset_;
};

