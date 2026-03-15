#pragma once
#include <kotono_interface/Widget.h>
#include <kotono_common/Path.h>
class WAssetExplorer : public WWidget
{
public:
	WAssetExplorer();

	WidgetPtr Build() override;
	void Cleanup() override;

private:
	void OnMousePreviousPressed();
	void OnMouseNextPressed();

private:
	UPath path_;

	std::vector<UPath> navigatedPaths_;
	size currentPathIndex_;
};