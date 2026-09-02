#pragma once
#include "generated/AssetExplorerItem.generated.h"
#include <kotono_object/Widget.h>

#include <kotono_common/Path.h>
class WAssetExplorer;
class WColor;
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

	void OnFocused() override;
	void OnUnfocused() override;

protected:
	UPath path_;

private:
	UPtr<WAssetExplorer> assetExplorer_;
	OnClickedFunc onDoubleClicked_;

	b8 isSelected_;
	f32 lastClickedTime_;
	f32 doubleClickTreshold_;

	UPtr<WColor> background_;
};