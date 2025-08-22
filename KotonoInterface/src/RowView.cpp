#include "RowView.h"
#include "Row.h"

VRowView::VRowView(WRow* row) :
	VView(row),
	row_(row)
{
}

void VRowView::Build(UBuildSettings buildSettings)
{
	if (!row_)
	{
		return;
	}

	VView::Build(buildSettings);

	if (row_->rowSettings_.children.empty())
	{
		return;
	}

	const auto count{ row_->rowSettings_.children.size() };

	buildSettings.position.x -= buildSettings.bounds.x / 2.0f;

	if (count > 1)
	{
		buildSettings.bounds.x -= row_->rowSettings_.spacing * static_cast<float>(count - 1);
		buildSettings.bounds.x /= static_cast<float>(count);
	}

	buildSettings.position.x += buildSettings.bounds.x / 2.0f;

	for (auto* child : row_->rowSettings_.children)
	{
		if (child)
		{
			++buildSettings.layer;
			child->CreateView()->Build(buildSettings);
			buildSettings.position.x += buildSettings.bounds.x + row_->rowSettings_.spacing;
			//buildSettings.position.x += child->GetSize().x + rowSettings_.spacing;
		}
	}
}
