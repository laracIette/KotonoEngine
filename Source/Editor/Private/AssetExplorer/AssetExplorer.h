#pragma once
#include <Widget/Widget.h>

#include "AssetExplorer.generated.h"

class WSelectableList;
class WSelectableListItem;

class WAssetExplorer : public WWidget
{
	GENERATED()

public:
	WAssetExplorer();

protected:
	auto Build() -> WidgetPtr override;

public:
	auto OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8 override;
	auto OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) -> b8 override;

private:
	void Push(UPath const& path);
	
	auto CanNavigatePrevious() const -> b8;
	auto CanNavigateNext() const -> b8;

	void NavigatePrevious();
	void NavigateNext();

	auto MakeItems() -> USet<UObjectPtr<WSelectableListItem>>;
	void UpdateItemList();

private:
	UPath currentDirectory_;
	std::vector<UPath> navigatedPaths_;
	size currentPathIndex_;
	UObjectPtr<WSelectableList> itemList_;
};