#include "Image.h"
#include "Framework.h"

void KtImage::AddToRenderQueue2D(const glm::mat4& modelMatrix) const
{
    WindowViewport.SetOffset({ 0, 0 });
    WindowViewport.SetExtent(Framework.GetRenderer().GetSwapChainExtent());

    KtAddToRenderQueue2DArgs args{};
    args.Viewport = &WindowViewport;
    args.Shader = _shader;
    args.ObjectData = { modelMatrix };

    Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(args);
}

KtShader2D* KtImage::GetShader() const
{
    return _shader;
}

void KtImage::SetShader(KtShader2D* shader)
{
    _shader = shader;
}
