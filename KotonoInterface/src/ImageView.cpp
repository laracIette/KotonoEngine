#include "ImageView.h"
#include "Image.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderable2DProxy.h>
#include <kotono_framework/ImageTexture.h>
#include <kotono_framework/Viewport.h>

VImageView::VImageView(WImage* image) :
	VView(image)
{
}

void VImageView::Build(UBuildSettings buildSettings)
{
	if (!widget_)
	{
		return;
	}

	VView::Build(buildSettings);

	auto* asImage{ static_cast<WImage*>(widget_) };

	static const auto shaderPath = Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2D.ktshader)";

	asImage->imageProxy_ = Framework.GetRenderer().GetInterfaceRenderer().CreateProxy();
	asImage->imageProxy_->shader = Framework.GetShaderManager().Get(shaderPath);
	asImage->imageProxy_->renderable = Framework.GetImageTextureManager().Get(asImage->imageSettings_.path);
	asImage->imageProxy_->viewport = &WindowViewport;
	asImage->imageProxy_->layer = buildSettings.layer;
	asImage->imageProxy_->objectData.modelMatrix = GetModelMatrix(buildSettings);
#	if _DEBUG
		asImage->imageProxy_->source = this;
#	endif
	Framework.GetRenderer().GetInterfaceRenderer().Register(asImage->imageProxy_);
}
