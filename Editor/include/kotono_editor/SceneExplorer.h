#pragma once
#include "generated/SceneExplorer.generated.h"
#include <kotono_object/SceneWidget.h>
class WList;
enum class EGameState : u8;
class WSceneExplorer : public WSceneWidget
{
	GENERATED_WSCENEEXPLORER()

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void OnGameStateChanged(EGameState gameState) const;
	void PopulateItemList() const;

private: 
	UPtr<WList> itemList_;
};