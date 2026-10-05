#pragma once
#include <Widget/Widget.h>

#include "AssetExplorer.generated.h"

class WAssetExplorerItem;
class WHorizontalWrapList;

class WAssetExplorer : public WWidget
{
	GENERATED()

private:
	using AssetExplorerItem = UPtr<WAssetExplorerItem>;

public:
	WAssetExplorer();

protected:
	WidgetPtr Build() override;

public:
	auto OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8 override;
	auto OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) -> b8 override;

	void DeselectOthers(AssetExplorerItem const& item) const;

private:
	void Push(UPath const& path);

	void NavigatePrevious();
	void NavigateNext();

	auto MakeItems() -> USet<AssetExplorerItem>;
	void UpdateItemList();

private:
	UPath currentDirectory_;
	std::vector<UPath> navigatedPaths_;
	size currentPathIndex_;
	UPtr<WHorizontalWrapList> itemList_;
	USet<AssetExplorerItem> assetExplorerItems_;
};