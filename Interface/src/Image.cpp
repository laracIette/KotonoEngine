#include "Image.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/WindowViewport.h>
#include <kotono_framework/InterfaceRenderableProxy.h>
#include <kotono_framework/ImageTexture.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ImageTextureManager.h>

WImage::WImage(const ImageSettings& imageSettings) : 
	imageSettings_(imageSettings)
{
}

void WImage::DisplayInternal(DisplaySettings displaySettings)
{
	static const auto shaderPath = Framework.Path().Framework() / R"(shaders\shader2D.ktshader)";

	imageProxy_ = Framework.Renderer().GetInterfaceRenderer().CreateProxy();
	imageProxy_->shader = Framework.ShaderManager().Get(shaderPath);
	imageProxy_->renderable = Framework.ImageTextureManager().Get(imageSettings_.path);
	imageProxy_->layer = displaySettings.layer;
	imageProxy_->objectData.modelMatrix = ModelMatrix();
	imageProxy_->scissor = displaySettings.scissor;
#	ifdef _DEBUG
		imageProxy_->source = this;
#	endif
	Framework.Renderer().GetInterfaceRenderer().Register(imageProxy_);
}

void WImage::Cleanup()
{
	Framework.Renderer().GetInterfaceRenderer().Unregister(imageProxy_);
	Framework.Renderer().GetInterfaceRenderer().DeleteProxy(imageProxy_);

	WWidget::Cleanup();
}

