#pragma once
#include "Widget/Widget.h"

#include "SelectableList.generated.h"

class WList;
class WSelectableListItem;

class WSelectableList final : public WWidget
{
    GENERATED()

public:
    using ItemPtr = UObjectPtr<WSelectableListItem>;

public:
	WSelectableList();
	
protected:
	auto Build() -> WidgetPtr override;

public:
    auto OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8 override;
	
    auto GetChildren() const -> WidgetSet const&;
	void SetChildren(USet<ItemPtr> const& children);
	void AddChild(ItemPtr const& child);

private:
    void SetItemProperties(ItemPtr const& item);
    void DeselectItems();

private:
	UObjectPtr<WList> itemList_;
	USet<ItemPtr> items_;
    USet<ItemPtr> selectedItems_;
    WritableProperty(f32, spacing_, Spacing);
};
