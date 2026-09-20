#pragma once
#include "generated/AssetExplorer.generated.h"
#include <kotono_core/Widget.h>

#include <kotono_common/Path.h>
class WAssetExplorerItem;
class WHorizontalWrapList;
class WBox;
class WColor;
class WOffset;
class WAssetExplorer : public WWidget
{
	GENERATED_WASSETEXPLORER()

public:
	WAssetExplorer();

protected:
	WidgetPtr Build() override;

public:
	b8 OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) override;

	void DeselectOthers(UPtr<WAssetExplorerItem> const& item) const;

private:
	void Push(UPath const& path);

	void NavigatePrevious();
	void NavigateNext();

	auto MakeItems() -> USet<UPtr<WAssetExplorerItem>>;
	void UpdateItemList();

private:
	UPath path_;
	std::vector<UPath> navigatedPaths_;
	size currentPathIndex_;
	UPtr<WHorizontalWrapList> itemList_;
	USet<UPtr<WAssetExplorerItem>> assetExplorerItems_;

	UPtr<WOffset> selectOffset_;
	UPtr<WBox> selectBox_;
	UPtr<WColor> selectColor_;
};