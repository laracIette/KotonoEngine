#include "Image.h"
#include <glm/ext/matrix_transform.hpp>
#include <kotono_common/AssetManager.h>
#include <kotono_graphics/Color.h>
#include <kotono_graphics/DrawDataBufferData.h>
#include <kotono_graphics/DrawCall.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/ParametersBufferData.h>
#include <kotono_graphics/RenderContext.h>
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

	drawCallBuilder_.Unregister(GetRenderContext().GetRenderer());
}

void WImage::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	drawCallBuilder_.Register(GetRenderContext().GetRenderer(), ERenderBucket::Interface);

	drawCallBuilder_.GetDrawCall()->scissor = { 
		.offset = { displaySettings.scissor.offset.x, displaySettings.scissor.offset.y },
		.extent = { displaySettings.scissor.extent.x, displaySettings.scissor.extent.y },
	};
	drawCallBuilder_.GetDrawCall()->sortKey = GetLayer();

	if (UAsset shader{ SAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/shader2D.kasset") })
	{
		drawCallBuilder_.GetDrawCall()->pipeline = shader->GetPipeline();
	}

	if (UAsset model{ SAssetManager<UModel>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/rectangle.obj") })
	{
		drawCallBuilder_.GetDrawData()->vertexBufferAddress = model->GetVertexBufferAddress();
		drawCallBuilder_.GetDrawCall()->indexCount = model->GetIndexCount();
		drawCallBuilder_.GetDrawCall()->firstIndex = model->GetFirstIndex();
	}

	drawCallBuilder_.GetTransform()->modelMatrix = ModelMatrix();
	drawCallBuilder_.GetTransform()->normalMatrix = glm::identity<glm::mat4>();

	if (UAsset texture{ SAssetManager<UTexture>::Get(path_) })
	{
		drawCallBuilder_.GetParameters()->textures = { texture->GetIndex() };
	}
	drawCallBuilder_.GetParameters()->vectors = { Colors::White };
}

#include "generated/Image.generated.inl"
