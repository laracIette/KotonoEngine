#pragma once
#include "generated/SceneExplorer.generated.h"
#include <kotono_core/SceneWidget.h>
#include <span>
class TSceneObject;
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
	void PopulateItemList(std::span<UPtr<TSceneObject> const> sceneObjects) const;

private:
	UPtr<WList> itemList_;
};