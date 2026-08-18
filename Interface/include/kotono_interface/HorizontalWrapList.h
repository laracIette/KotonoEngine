#pragma once
#include "generated/HorizontalWrapList.generated.h"
#include <kotono_object/Widget.h>
class WHorizontalWrapListBody;
/// Defines an horizontal container for widgets that wraps the content to multiple rows
class WHorizontalWrapList final : public WWidget
{
	GENERATED_WHORIZONTALWRAPLIST()

protected:
	WidgetPtr Build() override;

public:
	f32 GetItemSpacing() const;
	f32 GetRowSpacing() const;
	WidgetSet const& GetChildren() const;

	void SetItemSpacing(f32 itemSpacing);
	void SetRowSpacing(f32 rowSpacing);
	void SetChildren(WidgetSet const& children);

private:
	UPtr<WHorizontalWrapListBody> body_;
};