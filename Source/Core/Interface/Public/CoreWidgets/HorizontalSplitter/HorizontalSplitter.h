#pragma once
#include <Widget/Widget.h>

#include <Containers/Map.h>

#include <HorizontalSplitter.generated.h>

class WRow;

class WHorizontalSplitter final : public WWidget
{
	GENERATED()
	
public:
	WHorizontalSplitter();
	
protected:
	WidgetPtr Build() override;
	
public:
	void SetChildren(WidgetSet const& children);
	void AddChild(WidgetPtr const& child);
	void RemoveChild(WidgetPtr const& child);
	
private:
	UPtr<WRow> row_;
	UMap<WidgetPtr, WidgetPtr> childSeparators_;
};