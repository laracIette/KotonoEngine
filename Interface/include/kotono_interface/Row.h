#pragma once
#include "generated/Row.generated.h"
#include "ChildrenOwner.h"
/// Defines an horizontal container for widgets
class WRow final : public WChildrenOwner
{
	GENERATED_WROW()

public:	
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

	EExpand GetExpand() const override;
	EFlex GetFlex() const override;

public:
	float GetSpacing() const;

	void SetSpacing(const float spacing);

protected:	
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	size GetExpandCount() const;

private:
	float spacing_;
};

