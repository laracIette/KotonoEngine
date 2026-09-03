#pragma once
#include "generated/GameStateButton.generated.h"
#include <kotono_core/SceneWidget.h>
class WColor;
enum class EGameState : u8;
class WGameStateButton : public WSceneWidget
{
	GENERATED_WGAMESTATEBUTTON()

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void OnKeyboardSpaceKeyPressed() const;
	void OnGameStateChanged(EGameState gameState) const;
	void SwitchPlayPause() const;

private:
	UPtr<WColor> playPauseBg_;
	UPtr<WColor> stopBg_;
};

