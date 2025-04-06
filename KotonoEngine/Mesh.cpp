#include "Mesh.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ModelManager.h>
#include <nlohmann/json.hpp>
#include "Engine.h"

void TMesh::Init()
{
    Base::Init();
    _spinTask = Engine.GetObjectManager().Create<OTask>();
    _spinTask->GetEventUpdate().AddListener(this, &TMesh::Spin);
    _spinTask->SetDuration(5.0f);
    Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED)
        .AddListener(_spinTask, &OTask::Start);
}

void TMesh::Update()
{
    Base::Update();
    AddToRenderQueue();
}

void TMesh::Cleanup()
{
    Base::Cleanup();
    _spinTask->SetIsDelete(true);
    Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED)
        .RemoveListener(this);
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

void TMesh::Spin()
{
    const float speed = 10.0f * Engine.GetTime().GetDelta();
    const glm::quat rotation = glm::quat(glm::radians(glm::vec3(0.0f, speed, 0.0f)));
    GetTransform().AddRotation(rotation);
}
