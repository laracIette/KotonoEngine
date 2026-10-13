#pragma once
#include "Splitter.h"

#include <HorizontalSplitter.generated.h>

class WHorizontalSplitter final : public WSplitter
{
	GENERATED()
	
public:
	WHorizontalSplitter();
	
protected:
	auto MakeSeparator() -> WidgetPtr override;
	
private:
	WidgetPtr previous_;
	WidgetPtr next_;
	f32 startPreviousWidth_;
	f32 startPreviousWeight_;
};