#pragma once
#include "Widget.h"
class WCenter : public WWidget
{
public:
	enum class Axis
	{
		All,
		Horizontal,
		Vertical,
	};

	struct CenterSettings
	{
		// default  = Axis::All
		Axis axis{ Axis::All };
		WWidget* child{ nullptr };
	};

	WCenter(const CenterSettings& centerSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	CenterSettings centerSettings_;
};

