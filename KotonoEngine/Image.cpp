#include "Image.h"
#include <kotono_framework/Framework.h>

void RImage::Update()
{
	AddToRenderQueue();
}

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
	args.Viewport = GetViewport();
	args.ObjectData = { GetRect().GetModelMatrix()};
	Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(args);
}
