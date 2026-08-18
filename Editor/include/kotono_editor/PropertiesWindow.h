#pragma once
#include "generated/PropertiesWindow.generated.h"
#include <kotono_object\Widget.h>
class WList;
class WObjectProperties;
class WPropertiesWindow : public WWidget
{
	GENERATED_WPROPERTIESWINDOW()

	using ValueChangedFunction = std::function<void(float)>;

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void OnSelectedObjectChanged(UPtr<KObject> const& object);

private:
	UPtr<WList> mainList_;
	UPtr<WObjectProperties> objectProperties_;
};

