#pragma once
#include "generated/ListBody.generated.h"
#include "ChildrenOwner.h"
class WListBody final : public WChildrenOwner
{
	GENERATED_WLISTBODY()

public:
	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

public:
	float GetSpacing() const;

	void SetSpacing(const float spacing);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	float spacing_;
};
