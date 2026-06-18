#pragma once
#include "generated/SceneExplorer.generated.h"
#include <kotono_interface/Widget.h>
class WList;
enum class EGameState : u8;
class WSceneExplorer : public WWidget
{
	GENERATED_WSCENEEXPLORER()

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

private:
	void OnGameStateChanged(const EGameState gameState);
	void PopulateItemList() const;

private: 
	UPtr<WList> itemList_;
};