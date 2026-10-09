#pragma once
#include <SceneWidget/SceneWidget.h>

#include "SingleViewport.generated.h"

enum class EGameState : u8;
class WSwitcher;

class WSingleViewport final : public WSceneWidget
{
private:
	GENERATED()

protected:
	WidgetPtr Build() override;
	
public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;
	
private:
	void OnGameStateChanged(EGameState gameState) const;
	
private:
	UPtr<WSwitcher> switcher_;
};
