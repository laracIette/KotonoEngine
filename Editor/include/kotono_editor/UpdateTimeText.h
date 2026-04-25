#pragma once
#include "generated/UpdateTimeText.generated.h"
#include <kotono_interface/Widget.h>
class WText;
class WUpdateTimeText : public WWidget
{
	GENERATED_WUPDATETIMETEXT()

public:
	WidgetPtr Build() override;

	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

private:
	void UpdateText() const;

private:
	UPtr<WText> text_;
};

