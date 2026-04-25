#pragma once
#include "generated/SceneExplorer.generated.h"
#include <kotono_interface/Widget.h>
enum class EGameState : u8;
class WSceneExplorer : public WWidget
{
	GENERATED_WSCENEEXPLORER()

public:
	WidgetPtr Build() override;

	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

private:
	void OnGameStateChanged(const EGameState gameState);
};