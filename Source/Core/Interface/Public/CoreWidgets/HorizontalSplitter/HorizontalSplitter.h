#pragma once
#include <Widget/Widget.h>

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
	
private:
	UPtr<WRow> row_;
};