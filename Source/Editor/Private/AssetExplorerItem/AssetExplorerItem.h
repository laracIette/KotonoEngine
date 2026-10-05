#pragma once
#include <Widget/Widget.h>

#include "AssetExplorerItem.generated.h"

class WAssetExplorer;

class WAssetExplorerItem : public WWidget
{
	GENERATED()

public:
	using OpenedCallback = std::function<void(UPath const&)>;

public:
	WAssetExplorerItem(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path, OpenedCallback const& onopened);

protected:
	WidgetPtr Build() override;

public:
	void Select();
	void Deselect();

protected:
	UPath path_;

private:
	UPtr<WAssetExplorer> assetExplorer_;
	OpenedCallback onOpened_;

	b8 isSelected_;
	f32 lastClickedTime_;
	f32 openTreshold_;
};