#include "Image.h"
#include "Framework.h"

void KtImage::AddToRenderQueue2D(const glm::mat4& modelMatrix) const
{
    Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(
        _shader, { modelMatrix }
    );
}

KtShader2D* KtImage::GetShader() const
{
    return _shader;
}

void KtImage::SetShader(KtShader2D* shader)
{
    _shader = shader;
}
