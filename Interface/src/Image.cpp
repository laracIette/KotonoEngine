#include "Image.h"
#include <kotono_common/log.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Texture.h>
#include <kotono_graphics/TextureManager.h>
#include <kotono_graphics/InterfaceProxy.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_platform/WindowViewport.h>

WImage::WImage(const ImageSettings& imageSettings) : 
	imageSettings_(imageSettings)
{
}

void WImage::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	imageProxy_ = Renderer.InterfaceRenderer().CreateProxy();
	Renderer.InterfaceRenderer().RegisterProxy(imageProxy_);

	imageProxy_->ScheduleUpdate(
		[this, displaySettings](UInterfaceProxy::Data& data)
		{
			data.shader = ShaderManager.Get("${ENGINE_DIRECTORY}/Graphics/shaders/shader2D.ktshader");
			data.renderable = TextureManager.Get(imageSettings_.path);
			data.layer = displaySettings.layer;
			data.objectData.modelMatrix = ModelMatrix();
			data.scissor = displaySettings.scissor;
		}
	);
}

void WImage::Cleanup()
{
	Renderer.InterfaceRenderer().UnregisterProxy(imageProxy_);
	Renderer.InterfaceRenderer().DeleteProxy(imageProxy_);

	WWidget::Cleanup();
}

