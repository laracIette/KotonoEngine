#pragma once
#include "generated/UpdateTimeText.generated.h"
#include <kotono_core/Widget.h>
class WText;
class WUpdateTimeText : public WWidget
{
	GENERATED_WUPDATETIMETEXT()

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void UpdateText() const;

private:
	UPtr<WText> text_;
};

