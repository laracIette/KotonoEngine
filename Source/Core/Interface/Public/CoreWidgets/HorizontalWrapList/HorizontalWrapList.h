#pragma once
#include "WidgetContainer/WidgetContainer.h"

#include "HorizontalWrapList.generated.h"

class WHorizontalWrapListBody;
class WScrollable;

/// Defines an horizontal container for widgets that wraps the content to multiple rows
class WHorizontalWrapList final : public WWidgetContainer
{
	GENERATED()
	
public:
	using ItemSelectedCallback = UEvent<WidgetPtr>;

public:
	WHorizontalWrapList();
	~WHorizontalWrapList() override;

protected:
	auto Build() -> WidgetPtr override;

public:
	auto GetChildren() const -> WidgetSet const& override;
	void SetChildren(WidgetSet const& children) override;
	void AddChild(WidgetPtr const& child) override;

	auto GetItemSpacing() const -> f32;
	auto GetRowSpacing() const -> f32;
	void SetItemSpacing(f32 itemSpacing) const;
	void SetRowSpacing(f32 rowSpacing) const;

private:
	UObjectPtr<WScrollable> scrollable_;
	UObjectPtr<WHorizontalWrapListBody> body_;
};