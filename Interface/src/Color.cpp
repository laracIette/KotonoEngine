#include "Color.h"
#include <kotono_platform/WindowViewport.h>
#include <kotono_graphics/InterfaceRenderableProxy.h>
#include <kotono_graphics/ImageTexture.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/ImageTextureManager.h>

WColor::WColor(const ColorSettings& colorSettings) :
	colorSettings_(colorSettings)
{
}

void WColor::DisplayInternal(DisplaySettings displaySettings)
{
	static const auto shaderPath = Path.Framework() / R"(shaders\flatColor2D.ktshader)";
	static const auto imagePath = Path.Framework() / R"(assets\textures\white_texture.jpg)";

	colorProxy_ = Renderer.GetInterfaceRenderer().CreateProxy();
	colorProxy_->shader = ShaderManager.Get(shaderPath);
	colorProxy_->renderable = ImageTextureManager.Get(imagePath);
	colorProxy_->layer = displaySettings.layer;
	colorProxy_->objectData.modelMatrix = ModelMatrix();
	colorProxy_->objectData.color = colorSettings_.color;
	colorProxy_->scissor = displaySettings.scissor;
#	ifdef _DEBUG
		colorProxy_->source = this;
#	endif
	Renderer.GetInterfaceRenderer().Register(colorProxy_);
}

void WColor::Cleanup()
{
	Renderer.GetInterfaceRenderer().Unregister(colorProxy_);
	Renderer.GetInterfaceRenderer().DeleteProxy(colorProxy_);

	WWidget::Cleanup();
}
