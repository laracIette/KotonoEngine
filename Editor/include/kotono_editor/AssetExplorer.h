#pragma once
#include "generated/AssetExplorer.generated.h"
#include <kotono_object/Widget.h>
#include <kotono_common/Path.h>
class WHorizontalWrapList;
class WAssetExplorer : public WWidget
{
	GENERATED_WASSETEXPLORER()

public:
	WAssetExplorer();

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void Push(const UPath& path);

	void NavigatePrevious();
	void NavigateNext();

	void PopulateItemList();
	
	void OnMousePreviousButtonPressed();
	void OnMouseNextButtonPressed();

private:
	UPath path_;
	std::vector<UPath> navigatedPaths_;
	size currentPathIndex_;
	UPtr<WHorizontalWrapList> itemList_;
};