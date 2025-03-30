#include "Mesh.h"
#include <kotono_framework/Framework.h>

void TMesh::Update()
{
    AddToRenderQueue();
}

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

void TMesh::AddToRenderQueue() const
{
    KtAddToRenderQueue3DArgs args{};
    args.Shader = _shader;
    args.Model = _model;
    args.Viewport = GetViewport();
    args.ObjectData = { GetTransform().GetModelMatrix() };
	Framework.GetRenderer().GetRenderer3D().AddToRenderQueue(args);
}
