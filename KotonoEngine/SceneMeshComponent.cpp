#include "SceneMeshComponent.h"
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

void KSceneMeshComponent::Construct()
{
    Base::Construct();

    _spinTask = Engine.GetObjectManager().Create<KTask>();

    if (!WireframeShader)
    {
        const auto path = Framework.GetPath().GetFrameworkPath() / R"(shaders\wireframe3D.ktshader)";
        WireframeShader = Framework.GetShaderManager().Create(path);
    }
}

void KSceneMeshComponent::Init()
{
    Base::Init();

    _spinTask->SetDuration(5.0f);
    _spinTask->ListenEvent(Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED), &KTask::Start);
    
    ListenEvent(_spinTask->GetEventUpdate(), &KSceneMeshComponent::Spin);
    ListenEvent(Engine.GetObjectManager().GetEventDrawSceneObjects(), &KSceneMeshComponent::AddModelToRenderQueue);
    ListenEvent(Engine.GetObjectManager().GetEventDrawSceneObjectWireframes(), &KSceneMeshComponent::AddWireframeToRenderQueue);
}

void KSceneMeshComponent::Update()
{
    Base::Update();
}

void KSceneMeshComponent::Cleanup()
{
    Base::Cleanup();

    _spinTask->SetIsDelete(true);
}

KtShader* KSceneMeshComponent::GetShader() const
{
    return shader_;
}

KtModel* KSceneMeshComponent::GetModel() const
{
    return _model;
}

void KSceneMeshComponent::SetShader(KtShader* shader)
{
    shader_ = shader;
}

void KSceneMeshComponent::SetModel(KtModel* model)
{
    _model = model;
}

void KSceneMeshComponent::SerializeTo(nlohmann::json& json) const
{
    Base::SerializeTo(json);
    json["shader"] = shader_ ? shader_->GetPath() : "";
    json["model"] = _model ? _model->GetPath() : "";
}

void KSceneMeshComponent::DeserializeFrom(const nlohmann::json& json)
{
    Base::DeserializeFrom(json);
    shader_ = Framework.GetShaderManager().Get(json["shader"]);
    _model = Framework.GetModelManager().Get(json["model"]);
}

void KSceneMeshComponent::AddModelToRenderQueue()
{
    KtAddToRenderQueue3DArgs args{};
    args.Shader = shader_;
    args.Renderable = _model;
    args.Viewport = GetOwner()->GetViewport();
    args.ObjectData.Model = GetTransform().GetModelMatrix();
    Framework.GetRenderer().GetRenderer3D().AddToRenderQueue(args);
}

void KSceneMeshComponent::AddWireframeToRenderQueue()
{
    KtAddToRenderQueue3DArgs args{};
    args.Shader = WireframeShader;
    args.Renderable = _model;
    args.Viewport = GetOwner()->GetViewport();
    args.ObjectData.Model = GetTransform().GetModelMatrix();
    Framework.GetRenderer().GetRenderer3D().AddToRenderQueue(args);
}

void KSceneMeshComponent::Spin()
{
    const float speed = 10.0f * Engine.GetTime().GetDelta();
    const glm::quat rotation = glm::quat(glm::radians(glm::vec3(0.0f, speed, 0.0f)));
    GetTransform().AddRotation(rotation);
}
