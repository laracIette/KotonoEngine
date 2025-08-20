#pragma once
#include "Widget.h"
class WCenter : public WWidget
{
public:
	enum class Direction
	{
		Horizontal,
		Vertical,
		All,
	};

	struct CenterSettings
	{
		Direction direction{ Direction::All };
		WWidget* child{ nullptr };
	};

	WCenter(const CenterSettings& centerSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	CenterSettings centerSettings_;
};

