#include "Image.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderable2DProxy.h>
#include <kotono_framework/ImageTexture.h>
#include <kotono_framework/Viewport.h>

WImage::WImage(const ImageSettings& imageSettings) : 
	imageSettings_(imageSettings)
{
}

void WImage::Build(BuildSettings buildSettings)
{
	WWidget::Build(buildSettings);

	const auto shaderPath = Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2D.ktshader)";

	imageProxy_ = Framework.GetRenderer().GetInterfaceRenderer().CreateProxy();
	imageProxy_->shader = Framework.GetShaderManager().Get(shaderPath);
	imageProxy_->renderable = Framework.GetImageTextureManager().Get(imageSettings_.path);
	imageProxy_->viewport = &WindowViewport;
	imageProxy_->layer = buildSettings.layer;
	imageProxy_->objectData.modelMatrix = GetModelMatrix();
#if _DEBUG
	imageProxy_->source = this;
#endif
	Framework.GetRenderer().GetInterfaceRenderer().Register(imageProxy_);
}

void WImage::Destroy()
{
	Framework.GetRenderer().GetInterfaceRenderer().Unregister(imageProxy_);
	Framework.GetRenderer().GetInterfaceRenderer().DeleteProxy(imageProxy_);
}

