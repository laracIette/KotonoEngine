#pragma once
#include "generated/SceneExplorerRemoveButton.generated.h"
#include <kotono_object/SceneWidget.h>
class WButton;
class WColor;
class WSceneExplorerRemoveButton final : public WSceneWidget
{
	GENERATED_WSCENEEXPLORERREMOVEBUTTON()

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void OnSelectedObjectChanged(const UPtr<KObject> object) const;

private:
	UPtr<WColor> bg_;
	UPtr<WButton> button_;
};

