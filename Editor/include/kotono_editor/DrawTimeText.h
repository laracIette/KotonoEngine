#pragma once
#include "generated/DrawTimeText.generated.h"
#include <kotono_object/Widget.h>
class WText;
class WDrawTimeText : public WWidget
{
	GENERATED_WDRAWTIMETEXT()

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

