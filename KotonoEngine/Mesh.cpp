#include "Mesh.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/ModelManager.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/Model.h>
#include <nlohmann/json.hpp>
#include "Engine.h"
#include "ObjectManager.h"
#include "Task.h"
#include "Time.h"

static KtShader* WireframeShader = nullptr;

void TMesh::Construct()
{
    Base::Construct();

    _spinTask = Engine.GetObjectManager().Create<KTask>();

    if (!WireframeShader)
    {
        const auto path = Framework.GetPath().GetFrameworkPath() / R"(shaders\wireframe3D.ktshader)";
        WireframeShader = Framework.GetShaderManager().Create(path);
    }
}

void TMesh::Init()
{
    Base::Init();

    _spinTask->SetDuration(5.0f);
    _spinTask->ListenEvent(Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED), &KTask::Start);
    
    ListenEvent(_spinTask->GetEventUpdate(), &TMesh::Spin);
    ListenEvent(Engine.GetObjectManager().GetEventDrawSceneObjects(), &TMesh::AddModelToRenderQueue);
    ListenEvent(Engine.GetObjectManager().GetEventDrawSceneObjectWireframes(), &TMesh::AddWireframeToRenderQueue);
}

void TMesh::Update()
{
    Base::Update();
}

void TMesh::Cleanup()
{
    Base::Cleanup();

    _spinTask->SetIsDelete(true);
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

void TMesh::AddModelToRenderQueue()
{
    KtAddToRenderQueue3DArgs args{};
    args.Shader = _shader;
    args.Renderable = _model;
    args.Viewport = GetViewport();
    args.ObjectData.Model = GetTransform().GetModelMatrix();
    Framework.GetRenderer().GetRenderer3D().AddToRenderQueue(args);
}

void TMesh::AddWireframeToRenderQueue()
{
    KtAddToRenderQueue3DArgs args{};
    args.Shader = WireframeShader;
    args.Renderable = _model;
    args.Viewport = GetViewport();
    args.ObjectData.Model = GetTransform().GetModelMatrix();
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
