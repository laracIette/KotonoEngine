#pragma once
#include "generated/SceneExplorer.generated.h"
#include <kotono_interface/Widget.h>
enum class EGameState : char;
class WSceneExplorer : public WWidget
{
	GENERATED_WSCENEEXPLORER()

public:
	WidgetPtr Build() override;
	void Cleanup() override;

private:
	void OnGameStateChanged(const EGameState gameState);
};