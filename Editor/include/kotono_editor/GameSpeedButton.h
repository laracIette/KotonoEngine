#pragma once
#include "generated/GameSpeedButton.generated.h"
#include <kotono_core/SceneWidget.h>
class WGameSpeedButton final : public WSceneWidget
{
	GENERATED_WGAMESPEEDBUTTON()

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void OnTimeScaleChanged(f32 timeScale) const;

};