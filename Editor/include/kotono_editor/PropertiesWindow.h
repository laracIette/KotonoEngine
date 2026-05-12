#pragma once
#include "generated/PropertiesWindow.generated.h"
#include <kotono_interface\Widget.h>
class WList;
class WObjectProperties;
class WPropertiesWindow : public WWidget
{
	GENERATED_WPROPERTIESWINDOW()

	using ValueChangedFunction = std::function<void(float)>;

public:
	WidgetPtr Build() override;

	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

private:
	void OnSelectedObjectChanged(const UPtr<KObject> object);

private:
	UPtr<WList> mainList_;
	UPtr<WObjectProperties> objectProperties_;
};

