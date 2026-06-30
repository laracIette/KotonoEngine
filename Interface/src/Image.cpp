#include "Image.h"
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_graphics/DrawDataBufferData.h>
#include <kotono_graphics/DrawCall.h>
#include <kotono_graphics/InterfaceProxy.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/ParametersBufferData.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Texture.h>
#include <kotono_graphics/TransformBufferData.h>

WImage::WImage(const UPath& path)
	: path_{ path }
	, drawCallBuilder_{}
{}

void WImage::Remove()
{
	Base::Remove();

	if (imageProxy_)
	{
		Renderer.InterfaceRenderer().UnregisterProxy(imageProxy_);
		Renderer.InterfaceRenderer().DeleteProxy(imageProxy_);
		imageProxy_ = nullptr;
	}

	drawCallBuilder_.Unregister();
	//KT_LOG(KT_LOG_COMPILE_TIME_LEVEL, "Interface", "{0}", GetName());
}

void WImage::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (!imageProxy_)
	{
		imageProxy_ = Renderer.InterfaceRenderer().CreateProxy();
		Renderer.InterfaceRenderer().RegisterProxy(imageProxy_);
	}

	drawCallBuilder_.Register();
	//KT_LOG(KT_LOG_COMPILE_TIME_LEVEL, "Interface", "{0}", GetName());
	

	drawCallBuilder_.GetDrawCall()->scissor = { 
		.offset = { displaySettings.scissor.offset.x, displaySettings.scissor.offset.y },
		.extent = { displaySettings.scissor.extent.x, displaySettings.scissor.extent.y },
	};
	drawCallBuilder_.GetDrawCall()->renderBucket = ERenderBucket::Interface;
	drawCallBuilder_.GetDrawCall()->sortKey = GetLayer();

	if (UAsset shader{ UAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/shaders/shader2D.ktshader") })
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

	if (UAsset texture{ UAssetManager<UTexture>::Get(path_) })
	{
		drawCallBuilder_.GetParameters()->textures = { texture->GetIndex() };
	}

	imageProxy_->ScheduleUpdate(
		[this, displaySettings](UInterfaceProxy::Data& data)
		{
			data.shader = UAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/shaders/shader2D.ktshader").Get();
			data.renderable = UAssetManager<UTexture>::Get(path_).Get();
			data.layer = displaySettings.layer;
			data.objectData.modelMatrix = ModelMatrix();
			data.scissor = displaySettings.scissor;
		}
	);
}

#include "generated/Image.generated.inl"
