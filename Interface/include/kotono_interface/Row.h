#pragma once
#include "generated/Row.generated.h"
#include "ChildrenOwner.h"
class WRow : public WChildrenOwner
{
	GENERATED_WROW()

public:
	struct RowSettings
	{
		/// default = 0.0f
		float spacing{ 0.0f };
		/// default = false
		bool shouldWrap{ true };
		WidgetVector children{};
	};

	/// Defines an horizontal container for widgets
	WRow(const RowSettings& rowSettings);	

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

protected:
	RowSettings rowSettings_;
	
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	size GetFlexCount() const;
};

