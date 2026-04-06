#include "Image.h"
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/InterfaceProxy.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Texture.h>

WImage::WImage(const ImageSettings& imageSettings) 
	: imageSettings_(imageSettings)
	, isProxyRegistered_(false)
{
}

void WImage::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (!isProxyRegistered_)
	{
		isProxyRegistered_ = true;
		imageProxy_ = Renderer.InterfaceRenderer().CreateProxy();
		Renderer.InterfaceRenderer().RegisterProxy(imageProxy_);
	}

	imageProxy_->ScheduleUpdate(
		[this, displaySettings](UInterfaceProxy::Data& data)
		{
			data.shader = UAssetManager<KtShader>::Get("${ENGINE_DIRECTORY}/Graphics/shaders/shader2D.ktshader").Get();
			data.renderable = UAssetManager<KtTexture>::Get(imageSettings_.path).Get();
			data.layer = displaySettings.layer;
			data.objectData.modelMatrix = ModelMatrix();
			data.scissor = displaySettings.scissor;
		}
	);
}

void WImage::Cleanup()
{
	if (isProxyRegistered_)
	{
		isProxyRegistered_ = false;
		Renderer.InterfaceRenderer().UnregisterProxy(imageProxy_);
		Renderer.InterfaceRenderer().DeleteProxy(imageProxy_);
	}

	WWidget::Cleanup();
}

