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

	void OnDisplayed() override;
	void OnRemoved() override;

private:
	auto MakeItems(std::span<UObjectPtr<TSceneObject> const> sceneObjects) const -> WidgetSet;
	void UpdateItemList(std::span<UObjectPtr<TSceneObject> const> sceneObjects) const;

private:
	UObjectPtr<WList> itemList_;
};