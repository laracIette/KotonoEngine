#pragma once
#include "generated/SceneExplorerRemoveButton.generated.h"
#include <kotono_interface/Widget.h>
class WButton;
class WColor;
class WSceneExplorerRemoveButton final : public WWidget
{
	GENERATED_WSCENEEXPLORERREMOVEBUTTON()

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

private:
	void OnSelectedObjectChanged(const UPtr<KObject> object) const;

private:
	UPtr<WColor> bg_;
	UPtr<WButton> button_;
};

