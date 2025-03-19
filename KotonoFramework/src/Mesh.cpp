#include "Mesh.h"
#include "Framework.h"
#include "log.h"

void KtMesh::AddToRenderQueue3D(const glm::mat4& modelMatrix) const
{

    KtViewport viewport{};
    viewport.Offset = { 0, 0 };
    viewport.Extent = Framework.GetRenderer().GetSwapChainExtent();

    KtAddToRenderQueue3DArgs args{};
    args.Viewport = viewport;
    args.Shader = _shader;
    args.Model = _model;
    args.ObjectData = { modelMatrix };

    Framework.GetRenderer().GetRenderer3D().AddToRenderQueue(args);
}

KtShader3D* KtMesh::GetShader() const
{
    return _shader;
}

KtModel* KtMesh::GetModel() const
{
    return _model;
}

void KtMesh::SetShader(KtShader3D* shader)
{
    _shader = shader;
}

void KtMesh::SetModel(KtModel* model)
{
    _model = model;
}
