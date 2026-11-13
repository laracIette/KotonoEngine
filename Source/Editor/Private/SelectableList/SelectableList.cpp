#include "SelectableList.h"

#include "core_widgets.h"
#include "SelectableListItem.h"
#include "WidgetContainer/WidgetContainer.h"

WSelectableList::~WSelectableList()
{
    Delete(container_);
}

auto WSelectableList::Build() -> WidgetPtr
{
    Check(Throw, container_, "container_ is null");
	return (
		UCreate<WExpanded>{}()
		| (
		    container_
		    | Apply(&WWidgetContainer::SetChildren, items_)
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
    Check(Throw, container_, "container_ is null");
	return container_->GetChildren();
}

void WSelectableList::SetChildren(USet<UObjectPtr<Item>> const& children)
{	
    for (auto const& child : children)
    {
        SetItemProperties(child);
    }

    if (container_)
    {
	    container_->SetChildren(children);
    }

	items_ = children;

    selectedItems_.Clear();
}

void WSelectableList::AddChild(UObjectPtr<Item> const& child)
{
    SetItemProperties(child);

    if (container_)
    {
        container_->AddChild(child);
    }

    items_.Add(child);
}

void WSelectableList::SetItemProperties(UObjectPtr<Item> const& item)
{
    if (item)
    {
        item->onClicked_ = [this, item](EModifier modifier)
        {
            if (modifier == EModifier::Control && item->GetIsSelected().Get())
            {
                if (item->GetIsSelected().GetIsValue())
                {
                    item->SetIsSelected(false);
                }
                selectedItems_.Remove(item);
                if (item->onDeselected_)
                {
                    item->onDeselected_();
                }
            }
            else
            {
                if (modifier != EModifier::Control)
                {
                    DeselectItems();
                }

                if (item->GetIsSelected().GetIsValue())
                {
                    item->SetIsSelected(true);
                }
                selectedItems_.Add(item);
                if (item->onSelected_)
                {
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
        if (item->GetIsSelected().GetIsValue())
        {
            item->SetIsSelected(false);
        }
        if (item->onDeselected_)
        {
            item->onDeselected_();
        }
    }

    selectedItems_.Clear();
}

#include "SelectableList.generated.inl"
