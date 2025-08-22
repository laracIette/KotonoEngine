#include "ColorView.h"
#include "Color.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Viewport.h>
#include <kotono_framework/Renderable2DProxy.h>
#include <kotono_framework/ImageTexture.h>

VColorView::VColorView(WColor* color) :
	VView(color),
	color_(color)
{
}

void VColorView::Build(UBuildSettings buildSettings)
{
	if (!color_)
	{
		return;
	}

	VView::Build(buildSettings);

	static const auto shaderPath = Framework.GetPath().GetFrameworkPath() / R"(shaders\flatColor2D.ktshader)";
	static const auto imagePath = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)";

	color_->colorProxy_ = Framework.GetRenderer().GetInterfaceRenderer().CreateProxy();
	color_->colorProxy_->shader = Framework.GetShaderManager().Get(shaderPath);
	color_->colorProxy_->renderable = Framework.GetImageTextureManager().Get(imagePath);
	color_->colorProxy_->viewport = &WindowViewport;
	color_->colorProxy_->layer = buildSettings.layer;
	color_->colorProxy_->objectData.modelMatrix = GetModelMatrix(buildSettings);
	color_->colorProxy_->objectData.color = color_->colorSettings_.color;
#	if _DEBUG
		color_->colorProxy_->source = this;
#	endif
	Framework.GetRenderer().GetInterfaceRenderer().Register(color_->colorProxy_);
}


