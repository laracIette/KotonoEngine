#include "Mesh.h"
#include <kotono_framework/Framework.h>
#include <nlohmann/json.hpp>

void TMesh::Update()
{
    Base::Update();
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

void TMesh::SerializeTo(nlohmann::json& json) const
{
    Base::SerializeTo(json);
    json["shader"] = _shader ? _shader->GetPath() : "";
    json["model"] = _model ? _model->GetPath() : "";
}

void TMesh::DeserializeFrom(const nlohmann::json& json)
{
    Base::DeserializeFrom(json);
    _shader = Framework.GetShaderManager().Get(json["shader"]);
    _model = Framework.GetModelManager().Get(json["model"]);
}
