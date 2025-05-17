#include "MeshComponent.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ModelManager.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/Model.h>
#include <nlohmann/json.hpp>
#include "Engine.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "Task.h"
#include "Time.h"

static KtShader* WireframeShader = nullptr;

void KMeshComponent::Construct()
{
    Base::Construct();

    _spinTask = Engine.GetObjectManager().Create<KTask>();

    if (!WireframeShader)
    {
        const auto path = Framework.GetPath().GetFrameworkPath() / R"(shaders\wireframe3D.ktshader)";
        WireframeShader = Framework.GetShaderManager().Create(path);
    }
}

void KMeshComponent::Init()
{
    Base::Init();

    _spinTask->SetDuration(5.0f);
    _spinTask->ListenEvent(Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED), &KTask::Start);
    
    ListenEvent(_spinTask->GetEventUpdate(), &KMeshComponent::Spin);
    ListenEvent(Engine.GetObjectManager().GetEventDrawSceneObjects(), &KMeshComponent::AddModelToRenderQueue);
    ListenEvent(Engine.GetObjectManager().GetEventDrawSceneObjectWireframes(), &KMeshComponent::AddWireframeToRenderQueue);
}

void KMeshComponent::Update()
{
    Base::Update();
}

void KMeshComponent::Cleanup()
{
    Base::Cleanup();

    _spinTask->SetIsDelete(true);
}

KtShader* KMeshComponent::GetShader() const
{
    return _shader;
}

KtModel* KMeshComponent::GetModel() const
{
    return _model;
}

void KMeshComponent::SetShader(KtShader* shader)
{
    _shader = shader;
}

void KMeshComponent::SetModel(KtModel* model)
{
    _model = model;
}

void KMeshComponent::AddModelToRenderQueue()
{
    KtAddToRenderQueue3DArgs args{};
    args.Shader = _shader;
    args.Renderable = _model;
    args.Viewport = GetOwner()->GetViewport();
    args.ObjectData.Model = GetTransform().GetModelMatrix();
    Framework.GetRenderer().GetRenderer3D().AddToRenderQueue(args);
}

void KMeshComponent::AddWireframeToRenderQueue()
{
    KtAddToRenderQueue3DArgs args{};
    args.Shader = WireframeShader;
    args.Renderable = _model;
    args.Viewport = GetOwner()->GetViewport();
    args.ObjectData.Model = GetTransform().GetModelMatrix();
    Framework.GetRenderer().GetRenderer3D().AddToRenderQueue(args);
}

void KMeshComponent::SerializeTo(nlohmann::json& json) const
{
    Base::SerializeTo(json);
    json["shader"] = _shader ? _shader->GetPath() : "";
    json["model"] = _model ? _model->GetPath() : "";
}

void KMeshComponent::DeserializeFrom(const nlohmann::json& json)
{
    Base::DeserializeFrom(json);
    _shader = Framework.GetShaderManager().Get(json["shader"]);
    _model = Framework.GetModelManager().Get(json["model"]);
}

void KMeshComponent::Spin()
{
    const float speed = 10.0f * Engine.GetTime().GetDelta();
    const glm::quat rotation = glm::quat(glm::radians(glm::vec3(0.0f, speed, 0.0f)));
    GetTransform().AddRotation(rotation);
}
