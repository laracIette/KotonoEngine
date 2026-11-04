#include "SelectableList.h"

#include "SelectableListItem.h"

#include <core_widgets.h>

#include "enum_utils.h"

WSelectableList::WSelectableList()
	: items_{}
{
	itemList_ = UCreate<WList>{}();
}

auto WSelectableList::Build() -> WidgetPtr
{
	return (
		UCreate<WExpanded>{}()
		| (
			itemList_
		)
	);
}

auto WSelectableList::OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8
{
	if (button != EButton::Left)
	{
		return INPUT_UNHANDLED;
	}
	
	if (has_flag(modifier, EModifier::Control))
	{
		return INPUT_UNHANDLED;
	}
	
	for (auto const& item : items_)
	{
		item->Deactivate();
	}
	
	return INPUT_HANDLED;
}

auto WSelectableList::GetChildren() const -> WidgetSet const&
{
	return itemList_->GetChildren();
}

void WSelectableList::SetChildren(USet<UObjectPtr<WSelectableListItem>> const& children)
{	
	itemList_->SetChildren(children);
	items_ = children;
}

void WSelectableList::AddChild(UObjectPtr<WSelectableListItem> const& child)
{
	itemList_->AddChild(child);
	items_.Add(child);
}

#include "SelectableList.generated.inl"
