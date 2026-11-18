#pragma once

#include "SceneWidget/SceneWidget.h"

#include "SceneExplorer.generated.h"

class KSceneObject;
class WSelectableList;
class WSelectableListItem;
enum class EGameState : u8;

class WSceneExplorer : public WSceneWidget
{
	GENERATED()

protected:
	auto Build() -> WidgetPtr override;

	void OnDisplayed() override;
	void OnRemoved() override;

public:
    auto OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) -> b8 override;

private:
	auto MakeItems(USpan<UObjectPtr<KSceneObject> const> sceneObjects) const -> USet<UObjectPtr<WSelectableListItem>>;
	void UpdateItemList(USpan<UObjectPtr<KSceneObject> const> sceneObjects) const;

private:
	UObjectPtr<WSelectableList> itemList_;
};