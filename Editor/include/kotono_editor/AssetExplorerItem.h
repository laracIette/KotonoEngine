#pragma once
#include <kotono_interface/Widget.h>
#include <kotono_common/Path.h>
class WAssetExplorerItem : public WWidget
{
public:
	WAssetExplorerItem(const UPath& path);

	WidgetPtr Build() override;

private:
	const UPath path_;
};