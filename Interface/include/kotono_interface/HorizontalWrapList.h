#pragma once
#include "generated/HorizontalWrapList.generated.h"
#include "ChildrenOwner.h"
/// Defines an horizontal container for widgets that wraps the content to multiple rows
class WHorizontalWrapList final : public WChildrenOwner
{
	GENERATED_WHORIZONTALWRAPLIST()

	friend class WHorizontalWrapListBody;

public:
	WidgetPtr Build() override;

public:
	float GetItemSpacing() const;
	float GetRowSpacing() const;

	void SetItemSpacing(const float itemSpacing);
	void SetRowSpacing(const float rowSpacing); 

private:
	float itemSpacing_;
	float rowSpacing_;
};