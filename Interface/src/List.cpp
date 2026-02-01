#include "List.h"
#include "widgets.h"
#include "ListBody.h"
#include <kotono_common/log.h>
#include <kotono_input/Mouse.h>

WList::WList(const ListSettings& listSettings) :
	listSettings_(listSettings),
	offset_(0.0f)
{
}

WWidget* WList::Build()
{
	Mouse.EventVerticalScroll().AddListener(UDelegate(this, &WList::OnMouseVerticalScroll));

	return new WStack({
		.children = {
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

void WList::Cleanup()
{
	Mouse.EventVerticalScroll().RemoveListener(UDelegate(this, &WList::OnMouseVerticalScroll));

	WWidget::Cleanup();
}

void WList::OnMouseVerticalScroll(const float delta)
{
	if (!IsMouseHovering())
	{
		return;
	}

	SetState([this, delta]()
		{
			const auto maxOffset{ std::min(Size().y - GetDesiredSize().y, 0.0f) };
			offset_ += delta * 10.0f;
			offset_ = std::clamp(offset_, maxOffset, 0.0f);
		}
	);
}
