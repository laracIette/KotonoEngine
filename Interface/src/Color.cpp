#include "Color.h"
#include <kotono_common/AssetManager.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/InterfaceProxy.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Texture.h>

void WColor::Remove()
{
	Base::Remove();

	if (colorProxy_)
	{
		Renderer.InterfaceRenderer().UnregisterProxy(colorProxy_);
		Renderer.InterfaceRenderer().DeleteProxy(colorProxy_);
		colorProxy_ = nullptr;
	}
}

const UColor& WColor::GetColor() const
{
	return color_;
}

void WColor::SetColor(const UColor& color)
{
	color_ = color;
}

void WColor::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (!colorProxy_)
	{
		colorProxy_ = Renderer.InterfaceRenderer().CreateProxy();
		Renderer.InterfaceRenderer().RegisterProxy(colorProxy_);
	}

	colorProxy_->ScheduleUpdate(
		[this, displaySettings](UInterfaceProxy::Data& data)
		{
			data.shader = UAssetManager<KtShader>::Get("${ENGINE_DIRECTORY}/Graphics/shaders/flatColor2D.ktshader").Get();
			data.renderable = UAssetManager<KtTexture>::Get("${ENGINE_DIRECTORY}/Graphics/assets/textures/white_texture.jpg").Get();
			data.layer = displaySettings.layer;
			data.objectData.modelMatrix = ModelMatrix();
			data.objectData.color = color_;
			data.scissor = displaySettings.scissor;
		}
	);
}

#include "generated/Color.generated.inl"
