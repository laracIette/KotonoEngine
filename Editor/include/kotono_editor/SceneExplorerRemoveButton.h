#pragma once
#include "generated/SceneExplorerRemoveButton.generated.h"
#include <kotono_core/SceneWidget.h>
class TSceneObject;
class WButton;
class WSceneExplorerRemoveButton final : public WSceneWidget
{
	GENERATED_WSCENEEXPLORERREMOVEBUTTON()

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void OnSelectedObjectChanged(UPtr<TSceneObject> const& sceneObject) const;

private:
	UPtr<WButton> button_;
};

