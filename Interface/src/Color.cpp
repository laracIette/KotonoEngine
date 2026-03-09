#include "Color.h"
#include <kotono_platform/WindowViewport.h>
#include <kotono_graphics/InterfaceProxy.h>
#include <kotono_graphics/Texture.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/TextureManager.h>

WColor::WColor(const ColorSettings& colorSettings) 
	: colorSettings_(colorSettings)
	, isProxyRegistered_(false)
{
}

void WColor::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (!isProxyRegistered_)
	{
		isProxyRegistered_ = true;
		colorProxy_ = Renderer.InterfaceRenderer().CreateProxy();
		Renderer.InterfaceRenderer().RegisterProxy(colorProxy_);
	}

	colorProxy_->ScheduleUpdate(
		[this, displaySettings](UInterfaceProxy::Data& data)
		{
			data.shader = ShaderManager.Get("${ENGINE_DIRECTORY}/Graphics/shaders/flatColor2D.ktshader");
			data.renderable = TextureManager.Get("${ENGINE_DIRECTORY}/Graphics/assets/textures/white_texture.jpg");
			data.layer = displaySettings.layer;
			data.objectData.modelMatrix = ModelMatrix();
			data.objectData.color = colorSettings_.color;
			data.scissor = displaySettings.scissor;
		}
	);
}

void WColor::Cleanup()
{
	if (isProxyRegistered_)
	{
		isProxyRegistered_ = false;
		Renderer.InterfaceRenderer().UnregisterProxy(colorProxy_);
		Renderer.InterfaceRenderer().DeleteProxy(colorProxy_);
	}

	WWidget::Cleanup();
}
