#include "Color.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/WindowViewport.h>
#include <kotono_framework/InterfaceRenderableProxy.h>
#include <kotono_framework/ImageTexture.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ImageTextureManager.h>

WColor::WColor(const ColorSettings& colorSettings) :
	colorSettings_(colorSettings)
{
}

void WColor::DisplayInternal(DisplaySettings displaySettings)
{
	static const auto shaderPath = Framework.Path().FrameworkPath() / R"(shaders\flatColor2D.ktshader)";
	static const auto imagePath = Framework.Path().FrameworkPath() / R"(assets\textures\white_texture.jpg)";

	colorProxy_ = Framework.Renderer().GetInterfaceRenderer().CreateProxy();
	colorProxy_->shader = Framework.ShaderManager().Get(shaderPath);
	colorProxy_->renderable = Framework.ImageTextureManager().Get(imagePath);
	colorProxy_->layer = displaySettings.layer;
	colorProxy_->objectData.modelMatrix = ModelMatrix();
	colorProxy_->objectData.color = colorSettings_.color;
	colorProxy_->scissor = displaySettings.scissor;
#	ifdef _DEBUG
		colorProxy_->source = this;
#	endif
	Framework.Renderer().GetInterfaceRenderer().Register(colorProxy_);
}

void WColor::Cleanup()
{
	Framework.Renderer().GetInterfaceRenderer().Unregister(colorProxy_);
	Framework.Renderer().GetInterfaceRenderer().DeleteProxy(colorProxy_);

	WWidget::Cleanup();
}
