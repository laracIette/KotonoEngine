#pragma once
#include "Widget/Widget.h"

#include "SelectableList.generated.h"

class WSelectableListItem;
class WWidgetContainer;

class WSelectableList final : public WWidget
{
    GENERATED()

public:
    using Item = WSelectableListItem;

public:
    ~WSelectableList() override;
	
protected:
	auto Build() -> WidgetPtr override;

public:
    auto OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8 override;
	
    auto GetChildren() const -> WidgetSet const&;
	void SetChildren(USet<UObjectPtr<Item>> const& children);
	void AddChild(UObjectPtr<Item> const& child);

private:
    void SetItemProperties(UObjectPtr<Item> const& item);
    void DeselectItems();

private:
	USet<UObjectPtr<Item>> items_;
    USet<UObjectPtr<Item>> selectedItems_;
    WritableProperty(UObjectPtr<WWidgetContainer>, container_, Container);
};
