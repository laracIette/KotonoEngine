#include "Color.h"
#include <glm/ext/matrix_transform.hpp>
#include <kotono_common/AssetManager.h>
#include <kotono_graphics/DrawDataBufferData.h>
#include <kotono_graphics/DrawCall.h>
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

	drawCallBuilder_.Unregister();
}

void WColor::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	drawCallBuilder_.Register(ERenderBucket::Interface);

	drawCallBuilder_.GetDrawCall()->scissor = {
		.offset = { displaySettings.scissor.offset.x, displaySettings.scissor.offset.y },
		.extent = { displaySettings.scissor.extent.x, displaySettings.scissor.extent.y },
	};
	drawCallBuilder_.GetDrawCall()->sortKey = GetLayer();

	if (UAsset shader{ SAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/flatColor2D.kasset") })
	{
		drawCallBuilder_.GetDrawCall()->pipeline = shader->GetPipeline();
	}

	if (UAsset model{ SAssetManager<UModel>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/rectangle.obj") })
	{
		drawCallBuilder_.GetDrawData()->vertexBufferAddress = model->GetVertexBufferAddress();
		drawCallBuilder_.GetDrawCall()->indexCount = model->GetIndexCount();
		drawCallBuilder_.GetDrawCall()->firstIndex = 0;
	}

	drawCallBuilder_.GetTransform()->modelMatrix = ModelMatrix();
	drawCallBuilder_.GetTransform()->normalMatrix = glm::identity<glm::mat4>();

	drawCallBuilder_.GetParameters()->vectors = { color_ };
}

#include "generated/Color.generated.inl"
