#include "Color.h"
#include <kotono_platform/WindowViewport.h>
#include <kotono_graphics/InterfaceProxy.h>
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
	colorProxy_ = Renderer.InterfaceRenderer().CreateProxy();
	Renderer.InterfaceRenderer().RegisterProxy(colorProxy_);

	colorProxy_->ScheduleUpdate(
		[this, displaySettings](UInterfaceProxy::FrameData& frameData)
		{
			static const auto shaderPath{ KtPath::Graphics() / "shaders" / "flatColor2D.ktshader" };
			static const auto imagePath{ KtPath::Graphics() / "assets" / "textures" / "white_texture.jpg" };

			frameData.shader = ShaderManager.Get(shaderPath);
			frameData.renderable = ImageTextureManager.Get(imagePath);
			frameData.layer = displaySettings.layer;
			frameData.objectData.modelMatrix = ModelMatrix();
			frameData.objectData.color = colorSettings_.color;
			frameData.scissor = displaySettings.scissor;
		}
	);
}

void WColor::Cleanup()
{
	Renderer.InterfaceRenderer().UnregisterProxy(colorProxy_);
	Renderer.InterfaceRenderer().DeleteProxy(colorProxy_);

	WWidget::Cleanup();
}
