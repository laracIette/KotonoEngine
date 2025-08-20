#include "Row.h"

WRow::WRow(const RowSettings& rowSettings) :
	rowSettings_(rowSettings)
{
}

void WRow::Build(BuildSettings buildSettings)
{
	WWidget::Build(buildSettings);

	if (rowSettings_.children.empty())
	{
		return;
	}

	buildSettings.position.x -= buildSettings.bounds.x / 2.0f;

	if (const auto count{ rowSettings_.children.size() }; count > 1)
	{
		buildSettings.bounds.x -= rowSettings_.spacing * static_cast<float>(count - 1);
		buildSettings.bounds.x /= static_cast<float>(count);
	}

	buildSettings.position.x += buildSettings.bounds.x / 2.0f;

	for (auto* child : rowSettings_.children)
	{
		if (child)
		{
			++buildSettings.layer;
			child->Build(buildSettings);
			buildSettings.position.x += child->GetSize().x + rowSettings_.spacing;
		}
	}
}

void WRow::Destroy()
{
	for (auto* child : rowSettings_.children)
	{
		if (child)
		{
			child->Destroy();
		}
	}
}
