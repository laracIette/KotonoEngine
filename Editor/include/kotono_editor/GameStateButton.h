#pragma once
#include "generated/GameStateButton.generated.h"
#include <kotono_interface\Widget.h>
class WColor;
enum class EGameState : u8;
class WGameStateButton : public WWidget
{
	GENERATED_WGAMESTATEBUTTON()

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

private:
	void OnKeyboardSpaceKeyPressed() const;
	void OnGameStateChanged(const EGameState gameState) const;

private:
	UPtr<WColor> playPauseBg_;
	UPtr<WColor> stopBg_;
};

