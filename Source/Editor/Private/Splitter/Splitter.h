#pragma once
#include <Widget/Widget.h>

#include <Containers/Map.h>

#include "Splitter.generated.h"

class WChildrenOwner;

ABSTRACT class WSplitter : public WWidget
{
	GENERATED()
	
protected:
	WidgetPtr Build() final;
	
public:
	void SetChildren(WidgetSet const& children);
	void AddChild(WidgetPtr const& child);
	
protected:
	virtual auto MakeSeparator() -> WidgetPtr = 0;
	
private:
	void RemoveChild(WidgetPtr const& child);
	
protected:
	UObjectPtr<WChildrenOwner> ChildrenOwner;

private:
	UMap<WidgetPtr, WidgetPtr> childSeparators_;
};