#include "Image.h"
#include "ImageView.h"
#include <kotono_framework/Framework.h>

WImage::WImage(const ImageSettings& imageSettings) : 
	imageSettings_(imageSettings)
{
}

VView* WImage::CreateView()
{
	return new VImageView(this);
}

void WImage::Cleanup()
{
	Framework.GetRenderer().GetInterfaceRenderer().Unregister(imageProxy_);
	Framework.GetRenderer().GetInterfaceRenderer().DeleteProxy(imageProxy_);

	WWidget::Cleanup();
}

