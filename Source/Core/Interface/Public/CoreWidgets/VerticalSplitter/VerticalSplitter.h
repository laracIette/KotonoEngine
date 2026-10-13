#pragma once
#include "Splitter/Splitter.h"

#include "VerticalSplitter.generated.h"

class WVerticalSplitter final : public WSplitter
{
	GENERATED()
	
public:
	WVerticalSplitter();
	
protected:
	auto MakeSeparator() -> WidgetPtr override;
	
private:
	WidgetPtr previous_;
	WidgetPtr next_;
	f32 startPreviousHeight_;
	f32 startPreviousWeight_;
};
