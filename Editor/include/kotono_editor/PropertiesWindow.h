#pragma once
#include "generated/PropertiesWindow.generated.h"
#include <kotono_core/SceneWidget.h>
class TSceneObject;
class WList;
class WObjectProperties;
class WPropertiesWindow : public WSceneWidget
{
	GENERATED_WPROPERTIESWINDOW()

	using ValueChangedFunction = std::function<void(float)>;

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void OnSelectedObjectChanged(UPtr<TSceneObject> const& sceneObject);

private:
	UPtr<WList> mainList_;
	UPtr<WObjectProperties> objectProperties_;
};

