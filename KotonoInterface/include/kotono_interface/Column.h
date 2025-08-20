#pragma once
#include "Widget.h"
#include <vector>
class WColumn : public WWidget
{
	using Base = WWidget;

public:
	struct ColumnSettings
	{
		float spacing{ 0.0f };
		std::vector<WWidget*> children{};
	};

	WColumn(const WidgetSettings& widgetSettings, const ColumnSettings& columnSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	ColumnSettings columnSettings_;
};

