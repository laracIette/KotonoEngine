#pragma once
#include "Widget.h"
#include <vector>
class WStack : public WWidget
{
public:
	struct StackSettings
	{
		std::vector<WWidget*> children{};
	};

	WStack(const StackSettings& stackSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	StackSettings stackSettings_;
};

