#pragma once
#include "Widget.h"
class WRow : public WWidget
{
public:
	struct RowSettings
	{
		// default = 0.0f
		float spacing{ 0.0f };
		std::vector<WWidget*> children{};
	};

	WRow(const RowSettings& rowSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	RowSettings rowSettings_;
};

