#pragma once
#include <Widget/Widget.h>

#include "AssetExplorerItem.generated.h"

class WAssetExplorer;

ABSTRACT class WAssetExplorerItem : public WWidget
{
	GENERATED()

public:
	using OpenedCallback = std::function<void(UPath const&)>;

public:
	WAssetExplorerItem(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path, OpenedCallback const& onOpened);

protected:
	WidgetPtr Build() override;

public:
	void Select();
	void Deselect();

protected:
	UPtr<WAssetExplorer> assetExplorer_;
	UPath path_;

private:
	OpenedCallback onOpened_;

	b8 isSelected_;
	f32 lastClickedTime_;
	f32 openThreshold_;
};