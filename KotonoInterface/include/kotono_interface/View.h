#pragma once
#include "utils.h"
class WWidget;
/// Representation of a widget
class VView
{
public:
	VView(WWidget* widget);

	virtual ~VView() = default;

	virtual void Build(UBuildSettings buildSettings);

protected:
	WWidget* widget_;
};

