#include "Color.h"
#include <kotono_platform/WindowViewport.h>
#include <kotono_graphics/InterfaceProxy.h>
#include <kotono_graphics/Texture.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/TextureManager.h>

WColor::WColor(const ColorSettings& colorSettings) :
	colorSettings_(colorSettings)
{
}

void WColor::DisplayInternal(DisplaySettings displaySettings)
{
	colorProxy_ = Renderer.InterfaceRenderer().CreateProxy();
	Renderer.InterfaceRenderer().RegisterProxy(colorProxy_);

	colorProxy_->ScheduleUpdate(
		[this, displaySettings](UInterfaceProxy::Data& data)
		{
			static const auto shaderPath{ KtPath::Graphics() / "shaders" / "flatColor2D.ktshader" };
			static const auto imagePath{ KtPath::Graphics() / "assets" / "textures" / "white_texture.jpg" };

			data.shader = ShaderManager.Get(shaderPath);
			data.renderable = TextureManager.Get(imagePath);
			data.layer = displaySettings.layer;
			data.objectData.modelMatrix = ModelMatrix();
			data.objectData.color = colorSettings_.color;
			data.scissor = displaySettings.scissor;
		}
	);
}

void WColor::Cleanup()
{
	Renderer.InterfaceRenderer().UnregisterProxy(colorProxy_);
	Renderer.InterfaceRenderer().DeleteProxy(colorProxy_);

	WWidget::Cleanup();
}
