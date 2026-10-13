#include "Splitter/Splitter.h"

#include "ChildrenOwner/ChildrenOwner.h"

WidgetPtr WSplitter::Build()
{
	return (
		ChildrenOwner
		| Apply(&WChildrenOwner::SetOnDisowned, [this](WidgetPtr const& widget) { RemoveChild(widget); })
	);
}

void WSplitter::SetChildren(WidgetSet const& children)
{
	for (auto const& child : children)
	{
		AddChild(child);
	}
}

void WSplitter::AddChild(WidgetPtr const& child)
{
	if (!child)
	{
		Log(Warning, "child is null");
		return;
	}
	
	if (!ChildrenOwner->GetChildren().empty())
	{
		UPtr const separator{ MakeSeparator() };
		ChildrenOwner->AddChild(separator);
		childSeparators_.TryEmplace(child, separator);
	}
	ChildrenOwner->AddChild(child);
}

void WSplitter::RemoveChild(WidgetPtr const& child)
{
	if (!child)
	{
		Log(Warning, "child is null");
		return;
	}
	
	auto const it{ childSeparators_.Find(child) };
	if (childSeparators_.IsValidIterator(it))
	{
		UPtr const separator{ it->second };
		Check(Throw, separator, "separator is null");
		separator->Delete();
		
		childSeparators_.Remove(it);
	}
}

#include "Splitter.generated.inl"
