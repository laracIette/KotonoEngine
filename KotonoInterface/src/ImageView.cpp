#include "ImageView.h"
#include "Image.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderable2DProxy.h>
#include <kotono_framework/ImageTexture.h>
#include <kotono_framework/Viewport.h>

VImageView::VImageView(WImage* image) :
	VView(image),
	image_(image)
{
}

void VImageView::Build(UBuildSettings buildSettings)
{
	if (!image_)
	{
		return;
	}

	VView::Build(buildSettings);

	static const auto shaderPath = Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2D.ktshader)";

	image_->imageProxy_ = Framework.GetRenderer().GetInterfaceRenderer().CreateProxy();
	image_->imageProxy_->shader = Framework.GetShaderManager().Get(shaderPath);
	image_->imageProxy_->renderable = Framework.GetImageTextureManager().Get(image_->imageSettings_.path);
	image_->imageProxy_->viewport = &WindowViewport;
	image_->imageProxy_->layer = buildSettings.layer;
	image_->imageProxy_->objectData.modelMatrix = GetModelMatrix(buildSettings);
#	if _DEBUG
		image_->imageProxy_->source = this;
#	endif
	Framework.GetRenderer().GetInterfaceRenderer().Register(image_->imageProxy_);
}
