#pragma once
#include <kotono_interface/Widget.h>
#include <kotono_common/Path.h>
class WAssetExplorerItem : public WWidget
{
public:
	using OnClickedFunc = std::function<void(const UPath&)>;

public:
	WAssetExplorerItem(const UPath& path, const OnClickedFunc& onDoubleClicked);

	WidgetPtr Build() override;

protected:
	const UPath path_;

private:
	const OnClickedFunc onDoubleClicked_;

	bool isSelected_;
	float lastClickedTime_;
	const float doubleClickTreshold_;
};