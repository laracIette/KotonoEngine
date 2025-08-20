#include "Box.h"

WBox::WBox(BoxSettings boxSettings) :
	boxSettings_(boxSettings)
{
}

void WBox::Build(BuildSettings buildSettings)
{
	buildSettings.bounds = boxSettings_.size;
	WWidget::Build(buildSettings);

	if (boxSettings_.child)
	{
		++buildSettings.layer;
		boxSettings_.child->Build(buildSettings);
	}
}

void WBox::Destroy()
{
	if (boxSettings_.child)
	{
		boxSettings_.child->Destroy();
	}
}
