#pragma once
#include <SceneWidget/SceneWidget.h>

#include <span>

#include "SceneExplorer.generated.h"

class TSceneObject;
class WList;
enum class EGameState : u8;

class WSceneExplorer : public WSceneWidget
{
	GENERATED()

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void OnGameStateChanged(EGameState gameState) const;

	auto MakeItems(std::span<UPtr<TSceneObject> const> sceneObjects) const -> WidgetSet;
	void UpdateItemList(std::span<UPtr<TSceneObject> const> sceneObjects) const;

private:
	UPtr<WList> itemList_;
};