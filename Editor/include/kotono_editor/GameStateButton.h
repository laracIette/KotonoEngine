#pragma once
#include "generated/GameStateButton.generated.h"
#include <kotono_core/SceneWidget.h>
class WButton;
enum class EGameState : u8;
class WGameStateButton final : public WSceneWidget
{
	GENERATED_WGAMESTATEBUTTON()

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void OnGameStateChanged(EGameState gameState) const;

private:
	UPtr<WButton> playPauseButton_;
	UPtr<WButton> stopButton_;
};

