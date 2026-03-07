#include "HorizontalWrapList.h"
#include "HorizontalWrapListBody.h"
#include "widgets.h"

WHorizontalWrapList::WHorizontalWrapList(const HorizontalWrapListSettings& horizontalWrapListSettings)
	: horizontalWrapListSettings_(horizontalWrapListSettings)
{
}

WidgetPtr WHorizontalWrapList::Build()
{
	auto vec{ horizontalWrapListSettings_.children.Get() };

	return new WScrollable({
		.axis = EAxis::Vertical,
		.child = [this]() { 
			return new WHorizontalWrapListBody({
				.itemSpacing = horizontalWrapListSettings_.itemSpacing,
				.rowSpacing = horizontalWrapListSettings_.rowSpacing,
				.children = horizontalWrapListSettings_.children,
			});
		},
	});
}
