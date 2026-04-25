#pragma once
#include "generated/Column.generated.h"
#include "ChildrenOwner.h"
/// Defines a vertical container for widgets
class WColumn final : public WChildrenOwner
{
	GENERATED_WCOLUMN()

public:
	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

public:
	float GetSpacing() const;

	void SetSpacing(const float spacing);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	size GetFlexCount() const;

private:
	float spacing_;
};

