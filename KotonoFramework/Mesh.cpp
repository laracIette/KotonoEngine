#include "Mesh.h"

void KtMesh::AddToRenderQueue() const
{
    //renderer.queue[_shader].add(_model)
}

KtModel* KtMesh::GetModel() const
{
    return _model;
}

KtShader* KtMesh::GetShader() const
{
    return _shader;
}

void KtMesh::SetModel(KtModel* model)
{
    _model = model;
}

void KtMesh::SetShader(KtShader* shader)
{
    _shader = shader;
}
