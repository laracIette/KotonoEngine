#pragma once
#include "generated/DrawTimeText.generated.h"
#include <kotono_interface/Widget.h>
class WText;
class WDrawTimeText : public WWidget
{
	GENERATED_WDRAWTIMETEXT()

public:
	WidgetPtr Build() override;

	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

private: 
	void UpdateText() const;

private:
	UPtr<WText> text_;
};

