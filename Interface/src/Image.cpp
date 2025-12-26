#include "Image.h"
#include <kotono_platform/WindowViewport.h>
#include <kotono_graphics/InterfaceRenderableProxy.h>
#include <kotono_graphics/ImageTexture.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/ImageTextureManager.h>

WImage::WImage(const ImageSettings& imageSettings) : 
	imageSettings_(imageSettings)
{
}

void WImage::DisplayInternal(DisplaySettings displaySettings)
{
	static const auto shaderPath = KtPath::Graphics() / R"(shaders\shader2D.ktshader)";

	imageProxy_ = Renderer.InterfaceRenderer().CreateProxy();
	imageProxy_->shader = ShaderManager.Get(shaderPath);
	imageProxy_->renderable = ImageTextureManager.Get(imageSettings_.path);
	imageProxy_->layer = displaySettings.layer;
	imageProxy_->objectData.modelMatrix = ModelMatrix();
	imageProxy_->scissor = displaySettings.scissor;
#	ifdef _DEBUG
		imageProxy_->source = this;
#	endif
	Renderer.InterfaceRenderer().Register(imageProxy_);
}

void WImage::Cleanup()
{
	Renderer.InterfaceRenderer().Unregister(imageProxy_);
	Renderer.InterfaceRenderer().DeleteProxy(imageProxy_);

	WWidget::Cleanup();
}

