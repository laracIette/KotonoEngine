#include "List.h"
#include "widgets.h"
#include "ListBody.h"

WList::WList(const ListSettings& listSettings) 
	: listSettings_(listSettings)
{
}

WidgetPtr WList::Build()
{
	return new WScrollable({
		.axis = EAxis::Vertical,
		.child = [this]() { 
			return new WListBody({
				.spacing = listSettings_.spacing,
				.children = listSettings_.children,
			});
		},
	});
}
