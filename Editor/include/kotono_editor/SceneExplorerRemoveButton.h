#pragma once
#include "generated/SceneExplorerRemoveButton.generated.h"
#include <kotono_interface/Widget.h>
class WSceneExplorerRemoveButton final : public WWidget
{
	GENERATED_WSCENEEXPLORERREMOVEBUTTON()

public:
	WidgetPtr Build() override;

	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

private:
	void OnSelectedObjectChanged(const UPtr<KObject> object);
};

