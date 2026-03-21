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
	void Push(const UPath& path);

	void NavigatePrevious();
	void NavigateNext();

	void OnMousePreviousButtonPressed();
	void OnMouseNextButtonPressed();


private:
	UPath path_;

	std::vector<UPath> navigatedPaths_;
	size currentPathIndex_;
};