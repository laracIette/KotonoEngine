#pragma once
#include "generated/AssetExplorerItem.generated.h"
#include <kotono_core/Widget.h>

#include <kotono_common/Path.h>
class WAssetExplorer;
class WAssetExplorerItem : public WWidget
{
	GENERATED_WASSETEXPLORERITEM()

public:
	using OnClickedFunc = std::function<void(UPath const&)>;

public:
	WAssetExplorerItem(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path, OnClickedFunc const& onDoubleClicked);

protected:
	WidgetPtr Build() override;

public:
	void Select();
	void Deselect();

protected:
	UPath path_;

private:
	UPtr<WAssetExplorer> assetExplorer_;
	OnClickedFunc onDoubleClicked_;

	b8 isSelected_;
	f32 lastClickedTime_;
	f32 doubleClickTreshold_;
};