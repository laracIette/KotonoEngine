#pragma once
#include "generated/AssetExplorerItem.generated.h"
#include <kotono_interface/Widget.h>
#include <kotono_common/Path.h>
class WColor;
class WAssetExplorerItem : public WWidget
{
	GENERATED_WASSETEXPLORERITEM()

public:
	using OnClickedFunc = std::function<void(const UPath&)>;

public:
	WAssetExplorerItem(const UPath& path, const OnClickedFunc& onDoubleClicked);

protected:
	WidgetPtr Build() override;

protected:
	const UPath path_;

private:
	const OnClickedFunc onDoubleClicked_;

	bool isSelected_;
	float lastClickedTime_;
	float doubleClickTreshold_;

	UPtr<WColor> background_;
};