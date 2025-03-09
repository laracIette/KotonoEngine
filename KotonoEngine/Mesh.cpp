#include "Mesh.h"

KtShader* TMesh::GetShader() const
{
    return _shader;
}

KtModel* TMesh::GetModel() const
{
    return _model;
}

void TMesh::SetShader(KtShader* shader)
{
    _shader = shader;
}

void TMesh::SetModel(KtModel* model)
{
    _model = model;
}

void TMesh::Draw() const
{
	Framework.GetRenderer().AddToRenderQueue(
        _shader, _model, { _transform.GetModelMatrix() }
    );
}
