#include "SelectableList.h"

#include "SelectableListItem.h"

#include <core_widgets.h>

#include "enum_utils.h"

WSelectableList::WSelectableList()
    : spacing_{ 0.0f }
{
	itemList_ = UCreate<WList>{}();
}

auto WSelectableList::Build() -> WidgetPtr
{
	return (
		UCreate<WExpanded>{}()
		| (
			itemList_
			| Apply(&WList::SetSpacing, spacing_)
		)
	);
}

auto WSelectableList::OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8
{
	if (button != EButton::Left || inputState != EInputState::Pressed || modifier == EModifier::Control)
	{
		return INPUT_UNHANDLED;
	}

    DeselectItems();
	
	return INPUT_HANDLED;
}

auto WSelectableList::GetChildren() const -> WidgetSet const&
{
	return itemList_->GetChildren();
}

void WSelectableList::SetChildren(USet<ItemPtr> const& children)
{	
    for (auto const& child : children)
    {
        SetItemProperties(child);
    }

	itemList_->SetChildren(children);
	items_ = children;

    selectedItems_.Clear();
}

void WSelectableList::AddChild(ItemPtr const& child)
{
    SetItemProperties(child);

	itemList_->AddChild(child);
	items_.Add(child);
}

void WSelectableList::SetItemProperties(ItemPtr const& item)
{
    if (item)
    {
        item->selectableList_ = Ptr();
        item->onClicked_ = [this, item](EModifier modifier)
        {
            if (modifier == EModifier::Control && item->GetIsSelected()())
            {
                if (item->onDeselected_)
                {
                    selectedItems_.Remove(item);
                    item->onDeselected_();
                }
            }
            else
            {
                if (modifier != EModifier::Control)
                {
                    DeselectItems();
                }

                if (item->onSelected_)
                {
                    selectedItems_.Add(item);
                    item->onSelected_();
                }
            }
        };
    }
}

void WSelectableList::DeselectItems()
{
    for (auto const& item : selectedItems_)
    {
        Check(Throw, item, "item is null");
        if (item->onDeselected_)
        {
            item->onDeselected_();
        }
    }

    selectedItems_.Clear();
}

#include "SelectableList.generated.inl"
