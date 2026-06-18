#include "Image.h"
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/InterfaceProxy.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Texture.h>
#include <glm/gtx/string_cast.hpp>

void WImage::Display(UWidgetDisplaySettings displaySettings)
{
	Base::Display(displaySettings);
}

void WImage::Remove()
{
	Base::Remove();

	if (imageProxy_)
	{
		Renderer.InterfaceRenderer().UnregisterProxy(imageProxy_);
		Renderer.InterfaceRenderer().DeleteProxy(imageProxy_);
		imageProxy_ = nullptr;
	}
}

void WImage::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (!imageProxy_)
	{
		imageProxy_ = Renderer.InterfaceRenderer().CreateProxy();
		Renderer.InterfaceRenderer().RegisterProxy(imageProxy_);
	}

	imageProxy_->ScheduleUpdate(
		[this, displaySettings](UInterfaceProxy::Data& data)
		{
			data.shader = UAssetManager<KtShader>::Get("${ENGINE_DIRECTORY}/Graphics/shaders/shader2D.ktshader").Get();
			data.renderable = UAssetManager<KtTexture>::Get(path_).Get();
			data.layer = displaySettings.layer;
			data.objectData.modelMatrix = ModelMatrix();
			data.scissor = displaySettings.scissor;
		}
	);
}

#include "generated/Image.generated.inl"
