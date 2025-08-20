#include "Color.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Viewport.h>
#include <kotono_framework/Renderable2DProxy.h>
#include <kotono_framework/ImageTexture.h>

WColor::WColor(const ColorSettings& colorSettings) :
	colorSettings_(colorSettings)
{
}

void WColor::Build(BuildSettings buildSettings)
{
	const auto shaderPath = Framework.GetPath().GetFrameworkPath() / R"(shaders\flatColor2D.ktshader)";
	const auto imagePath = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)";

	colorProxy_ = Framework.GetRenderer().GetInterfaceRenderer().CreateProxy();
	colorProxy_->shader = Framework.GetShaderManager().Get(shaderPath);
	colorProxy_->renderable = Framework.GetImageTextureManager().Get(imagePath);
	colorProxy_->viewport = &WindowViewport;
	colorProxy_->layer = buildSettings.layer;
	colorProxy_->objectData.modelMatrix = GetModelMatrix(buildSettings);
	colorProxy_->objectData.color = colorSettings_.color;
#if _DEBUG
	colorProxy_->source = this;
#endif
	Framework.GetRenderer().GetInterfaceRenderer().Register(colorProxy_);
}

void WColor::Destroy()
{
	Framework.GetRenderer().GetInterfaceRenderer().Unregister(colorProxy_);
	Framework.GetRenderer().GetInterfaceRenderer().DeleteProxy(colorProxy_);
}
