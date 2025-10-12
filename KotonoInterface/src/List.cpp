#include "List.h"
#include "widgets.h"
#include "ListBody.h"
#include "log.h"

WList::WList(const ListSettings& listSettings) :
	listSettings_(listSettings),
	offset_(0.0f)
{
}

WWidget* WList::Build()
{
	return new WStack({
		.children = {
			new WButton({
				.onVerticalScroll = [this](const float delta) { 
					SetState([this, delta]() { 
						offset_ += delta * 10.0f; 
						offset_ = std::clamp(offset_, -INFINITY, 0.0f);
					});
				},
			}),
			new WOffset({
				.offset = { 0.0f, offset_ },
				.child = new WListBody({
					.spacing = listSettings_.spacing,
					.children = listSettings_.children,
				}),
			}),
		},
	});
}
