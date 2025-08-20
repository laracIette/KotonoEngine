#include "Column.h"

WColumn::WColumn(const ColumnSettings& columnSettings) : 
	columnSettings_(columnSettings)
{
}

void WColumn::Build(BuildSettings buildSettings)
{
	WWidget::Build(buildSettings);

	if (columnSettings_.children.empty())
	{
		return;
	}

	buildSettings.position.y -= buildSettings.bounds.y / 2.0f;

	if (const auto count{ columnSettings_.children.size() }; count > 1)
	{
		buildSettings.bounds.y -= columnSettings_.spacing * static_cast<float>(count - 1);
		buildSettings.bounds.y /= static_cast<float>(count);
	}

	buildSettings.position.y += buildSettings.bounds.y / 2.0f;

	for (auto* child : columnSettings_.children)
	{
		if (child)
		{
			++buildSettings.layer;
			child->Build(buildSettings);
			buildSettings.position.y += child->GetSize().y + columnSettings_.spacing;
		}
	}
}

void WColumn::Destroy()
{
	for (auto* child : columnSettings_.children)
	{
		if (child)
		{
			child->Destroy();
		}
	}
}
