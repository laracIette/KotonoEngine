#include "ColorView.h"
#include "Color.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Viewport.h>
#include <kotono_framework/Renderable2DProxy.h>
#include <kotono_framework/ImageTexture.h>

VColorView::VColorView(WColor* color) :
	VView(color)
{
}

void VColorView::Build(UBuildSettings buildSettings)
{
	if (!widget_)
	{
		return;
	}

	VView::Build(buildSettings);

	auto* asColor{ static_cast<WColor*>(widget_) };

	static const auto shaderPath = Framework.GetPath().GetFrameworkPath() / R"(shaders\flatColor2D.ktshader)";
	static const auto imagePath = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)";

	asColor->colorProxy_ = Framework.GetRenderer().GetInterfaceRenderer().CreateProxy();
	asColor->colorProxy_->shader = Framework.GetShaderManager().Get(shaderPath);
	asColor->colorProxy_->renderable = Framework.GetImageTextureManager().Get(imagePath);
	asColor->colorProxy_->viewport = &WindowViewport;
	asColor->colorProxy_->layer = buildSettings.layer;
	asColor->colorProxy_->objectData.modelMatrix = GetModelMatrix(buildSettings);
	asColor->colorProxy_->objectData.color = asColor->colorSettings_.color;
#	if _DEBUG
		asColor->colorProxy_->source = this;
#	endif
	Framework.GetRenderer().GetInterfaceRenderer().Register(asColor->colorProxy_);
}


