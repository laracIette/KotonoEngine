#include "Color.h"
#include <kotono_common/AssetManager.h>
#include <kotono_graphics/DrawDataBufferData.h>
#include <kotono_graphics/DrawCall.h>
#include <kotono_graphics/InterfaceProxy.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/ParametersBufferData.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Texture.h>
#include <kotono_graphics/TransformBufferData.h>

WColor::WColor(const UColor& color)
	: color_{ color }
	, drawCallBuilder_{}
{
}

void WColor::Remove()
{
	Base::Remove();

	if (colorProxy_)
	{
		Renderer.InterfaceRenderer().UnregisterProxy(colorProxy_);
		Renderer.InterfaceRenderer().DeleteProxy(colorProxy_);
		colorProxy_ = nullptr;
		drawCallBuilder_.Unregister();
	}

}

void WColor::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (!colorProxy_)
	{
		colorProxy_ = Renderer.InterfaceRenderer().CreateProxy();
		Renderer.InterfaceRenderer().RegisterProxy(colorProxy_);
		drawCallBuilder_.Register();
	}

	drawCallBuilder_.GetDrawCall()->scissor = {
		.offset = { displaySettings.scissor.offset.x, displaySettings.scissor.offset.y },
		.extent = { displaySettings.scissor.extent.x, displaySettings.scissor.extent.y },
	};
	drawCallBuilder_.GetDrawCall()->renderBucket = ERenderBucket::Interface;
	drawCallBuilder_.GetDrawCall()->sortKey = GetLayer();

	if (UAsset shader{ UAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/shaders/flatColor2D.ktshader") })
	{
		drawCallBuilder_.GetDrawCall()->pipeline = shader->GetGraphicsPipeline();
	}

	if (UAsset model{ UAssetManager<UModel>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/rectangle.obj") })
	{
		drawCallBuilder_.GetDrawCall()->vertexBufferAdress = model->GetVertexBufferAddress();
		drawCallBuilder_.GetDrawCall()->indexBuffer = model->GetIndexBuffer();
		drawCallBuilder_.GetDrawCall()->indexCount = model->GetIndexCount();
		drawCallBuilder_.GetDrawCall()->firstIndex = 0;
	}

	drawCallBuilder_.GetTransform()->modelMatrix = ModelMatrix();
	drawCallBuilder_.GetTransform()->normalMatrix = glm::identity<glm::mat4>();

	drawCallBuilder_.GetParameters()->vectors = { color_ };

	colorProxy_->ScheduleUpdate(
		[this, displaySettings](UInterfaceProxy::Data& data)
		{
			data.shader = UAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/shaders/flatColor2D.ktshader").Get();
			data.renderable = UAssetManager<UTexture>::Get("${ENGINE_DIRECTORY}/Graphics/assets/textures/white_texture.jpg").Get();
			data.layer = displaySettings.layer;
			data.objectData.modelMatrix = ModelMatrix();
			data.objectData.color = color_;
			data.scissor = displaySettings.scissor;
		}
	);
}

#include "generated/Color.generated.inl"
