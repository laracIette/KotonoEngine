#pragma once
#include "Widget/Widget.h"

#include "SelectableList.generated.h"

class WList;
class WSelectableListItem;

class WSelectableList final : public WWidget
{
    GENERATED()

public:
	WSelectableList();
	
protected:
	auto Build() -> WidgetPtr override;

public:
    auto OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8 override;
	
    auto GetChildren() const -> WidgetSet const&;
	void SetChildren(USet<UObjectPtr<WSelectableListItem>> const& children);
	void AddChild(UObjectPtr<WSelectableListItem> const& child);
	
private:
	UObjectPtr<WList> itemList_;
	USet<UObjectPtr<WSelectableListItem>> items_;
};
