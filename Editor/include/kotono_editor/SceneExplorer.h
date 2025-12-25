#pragma once
#include <kotono_interface/Widget.h>
enum class EGameState : char;
class WSceneExplorer : public WWidget
{
public:
	WWidget* Build() override;
	void Cleanup() override;

private:
	void OnGameStateChanged(const EGameState gameState);
};