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
	WidgetPtr previous_;
	WidgetPtr next_;
	f32 startPreviousWidth_;
	f32 startPreviousWeight_;
};