#include "Mesh.h"
#include "Framework.h"
#include "log.h"

void KtMesh::AddToRenderQueue3D(const glm::mat4& modelMatrix) const
{
    Framework.GetRenderer().AddToRenderQueue3D<KT_RENDER_LAYER_ENGINE>(
        _shader, _model, { modelMatrix }
    );
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
