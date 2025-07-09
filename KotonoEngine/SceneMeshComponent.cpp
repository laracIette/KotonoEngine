#include "SceneMeshComponent.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ModelManager.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/Model.h>
#include "log.h"
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

    spinTask_ = AddObject<KTask>();

    if (!WireframeShader)
    {
        const auto path = Framework.GetPath().GetFrameworkPath() / R"(shaders\wireframe3D.ktshader)";
        WireframeShader = Framework.GetShaderManager().Get(path);
    }
}

void KSceneMeshComponent::Init()
{
    Base::Init();

    InitSpin();
    InitModelProxy();

    const KtDelegate<> keyboardSKeyPressed(this, &KSceneMeshComponent::SetMobilityStatic);
    const KtDelegate<> keyboardDKeyPressed(this, &KSceneMeshComponent::SetMobilityDynamic);
    Framework.GetInputManager().GetKeyboard()
        .GetEvent(KT_KEY_S, KT_INPUT_STATE_PRESSED)
        .AddListener(keyboardSKeyPressed);
    Framework.GetInputManager().GetKeyboard()
        .GetEvent(KT_KEY_D, KT_INPUT_STATE_PRESSED)
        .AddListener(keyboardDKeyPressed);
}

void KSceneMeshComponent::Update()
{
    Base::Update();
}

void KSceneMeshComponent::Cleanup()
{
    Base::Cleanup();
    RemoveProxies();
}

KtShader* KSceneMeshComponent::GetShader() const
{
    return shader_;
}

KtModel* KSceneMeshComponent::GetModel() const
{
    return model_;
}

void KSceneMeshComponent::SetShader(KtShader* shader)
{
    shader_ = shader;
}

void KSceneMeshComponent::SetModel(KtModel* model)
{
    model_ = model;
}

void KSceneMeshComponent::SerializeTo(nlohmann::json& json) const
{
    Base::SerializeTo(json);
    json["shader"] = shader_ ? shader_->GetPath() : "";
    json["model"] = model_ ? model_->GetPath() : "";
}

void KSceneMeshComponent::DeserializeFrom(const nlohmann::json& json)
{
    Base::DeserializeFrom(json);
    shader_ = Framework.GetShaderManager().Get(json["shader"]);
    model_ = Framework.GetModelManager().Get(json["model"]);
}

void KSceneMeshComponent::SetMobility(const EMobility mobility)
{
    RemoveProxies();
    Base::SetMobility(mobility);
    RegisterProxies();
}

void KSceneMeshComponent::InitSpin()
{
    const KtDelegate<> startSpinDelegate(spinTask_, &KTask::Start);
    const KtDelegate<> spinMeshDelegate(this, &KSceneMeshComponent::Spin);
    spinTask_->SetDuration(5.0f);
    spinTask_->ListenEvent(Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED), startSpinDelegate);
    ListenEvent(spinTask_->GetEventUpdate(), spinMeshDelegate);
}

void KSceneMeshComponent::InitModelProxy()
{
    CreateModelProxy();
    RegisterProxies();
    const KtDelegate<> transformDelegate(this, &KSceneMeshComponent::MarkModelProxyTransformDirty);
    ListenEvent(GetEventUpdateTransform(), transformDelegate);
}

void KSceneMeshComponent::CreateModelProxy()
{
    proxy_.shader = shader_;
    proxy_.renderable = model_;
    proxy_.viewport = GetOwner()->GetViewport();
    proxy_.objectData.modelMatrix = GetModelMatrix();
}

void KSceneMeshComponent::MarkModelProxyTransformDirty()
{
    proxy_.isDirty = true;
    proxy_.objectData.modelMatrix = GetModelMatrix();
}

void KSceneMeshComponent::RegisterProxies()
{
    switch (GetMobility())
    {
    case EMobility::Dynamic:
    {
        Framework.GetRenderer().GetRenderer3D().RegisterDynamic(&proxy_);
        break;
    }
    case EMobility::Static:
    {
        Framework.GetRenderer().GetRenderer3D().RegisterStatic(&proxy_);
        break;
    }
    }
}

void KSceneMeshComponent::RemoveProxies()
{
    switch (GetMobility())
    {
    case EMobility::Dynamic:
    {
        Framework.GetRenderer().GetRenderer3D().RemoveDynamic(&proxy_);
        break;
    }
    case EMobility::Static:
    {
        Framework.GetRenderer().GetRenderer3D().RemoveStatic(&proxy_);
        break;
    }
    }
}

void KSceneMeshComponent::Spin()
{
    const float speed = 10.0f * Engine.GetTime().GetDelta();
    const glm::quat rotation = glm::quat(glm::radians(glm::vec3(0.0f, speed, 0.0f)));
    Rotate(rotation);
}

void KSceneMeshComponent::SetMobilityStatic()
{
    SetMobility(EMobility::Static);
    KT_LOG_KE(KT_LOG_COMPILE_TIME_LEVEL, "static");
}

void KSceneMeshComponent::SetMobilityDynamic()
{
    SetMobility(EMobility::Dynamic);
    KT_LOG_KE(KT_LOG_COMPILE_TIME_LEVEL, "dynamic");
}
