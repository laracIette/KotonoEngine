#pragma once
#include "generated/HorizontalWrapList.generated.h"
#include "Widget.h"
class WHorizontalWrapListBody;
/// Defines an horizontal container for widgets that wraps the content to multiple rows
class WHorizontalWrapList final : public WWidget
{
	GENERATED_WHORIZONTALWRAPLIST()

protected:
	WidgetPtr Build() override;

public:
	float GetItemSpacing() const;
	float GetRowSpacing() const;
	const WidgetPool& GetChildren() const;

	void SetItemSpacing(const float itemSpacing);
	void SetRowSpacing(const float rowSpacing);
	void SetChildren(const WidgetPool& children);

private:
	UPtr<WHorizontalWrapListBody> body_;
};