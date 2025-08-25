#include "Color.h"
#include "ColorView.h"
#include <kotono_framework/Framework.h>

WColor::WColor(const ColorSettings& colorSettings) :
	colorSettings_(colorSettings)
{
}

VView* WColor::CreateView()
{
	return new VColorView(this);
}

void WColor::Cleanup()
{
	Framework.GetRenderer().GetInterfaceRenderer().Unregister(colorProxy_);
	Framework.GetRenderer().GetInterfaceRenderer().DeleteProxy(colorProxy_);

	WWidget::Cleanup();
}
