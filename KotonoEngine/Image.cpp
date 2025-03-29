#include "Image.h"
#include <kotono_framework/Framework.h>

KtShader* RImage::GetShader() const
{
	return _shader;
}

void RImage::SetShader(KtShader* shader)
{
	_shader = shader;
}

void RImage::AddToRenderQueue() const
{
	KtAddToRenderQueue2DArgs args{};
	args.Shader = _shader;
	args.Viewport = &WindowViewport;
	args.ObjectData = { _rect.GetModelMatrix() };
	Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(args);
}
