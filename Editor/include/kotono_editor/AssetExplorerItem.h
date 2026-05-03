#pragma once
#include "generated/AssetExplorerItem.generated.h"
#include <kotono_interface/Widget.h>
#include <kotono_common/Path.h>
class WAssetExplorerItem : public WWidget
{
	GENERATED_WASSETEXPLORERITEM()

public:
	using OnClickedFunc = std::function<void(const UPath&)>;

public:
	WAssetExplorerItem(const UPath& path, const OnClickedFunc& onDoubleClicked);

	WidgetPtr Build() override;

	void Display(UWidgetDisplaySettings displaySettings) override;

protected:
	const UPath path_;

private:
	const OnClickedFunc onDoubleClicked_;

	bool isSelected_;
	float lastClickedTime_;
	float doubleClickTreshold_;
};