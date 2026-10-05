#pragma once
#include "AssetExplorerItem.generated.h"
#include <Widget/Widget.h>

class WAssetExplorer;

class WAssetExplorerItem : public WWidget
{
	GENERATED_WASSETEXPLORERITEM()

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