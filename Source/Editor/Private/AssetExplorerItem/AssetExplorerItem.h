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
	WAssetExplorerItem(UPath path, OpenedCallback onOpened);

protected:
	auto Build() -> WidgetPtr override;

public:
    void Open() const;

private:
	UObjectPtr<WAssetExplorer> assetExplorer_;
	UPath path_;

	OpenedCallback onOpened_;

	f32 lastClickedTime_;
	f32 openThreshold_;
};