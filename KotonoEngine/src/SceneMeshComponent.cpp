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
#include "TimeManager.h"

static KtShader* WireframeShader = nullptr;

KSceneMeshComponent::KSceneMeshComponent(TSceneObject* owner) :
    Base(owner)
{
    spinTask_ = Engine.GetObjectManager().Create<KTask>();

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

    Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_N, KT_INPUT_STATE_PRESSED).AddListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityStatic));
    Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_M, KT_INPUT_STATE_PRESSED).AddListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityDynamic));
}

void KSceneMeshComponent::Update()
{
    Base::Update();
}

void KSceneMeshComponent::Cleanup()
{
    Base::Cleanup();

    RemoveProxies();
    GetEventTransformUpdated().RemoveListener(KtDelegate(this, &KSceneMeshComponent::MarkModelProxyTransformDirty));

    Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED).RemoveListener(KtDelegate(spinTask_, &KTask::Start));
    spinTask_->Delete();
    
    Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_N, KT_INPUT_STATE_PRESSED).RemoveListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityStatic));
    Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_M, KT_INPUT_STATE_PRESSED).RemoveListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityDynamic));
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
    spinTask_->SetDuration(5.0f);
    Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED).AddListener(KtDelegate(spinTask_, &KTask::Start));
    spinTask_->GetEventUpdate().AddListener(KtDelegate(this, &KSceneMeshComponent::Spin));
}

void KSceneMeshComponent::InitModelProxy()
{
    CreateModelProxy();
    RegisterProxies();
    GetEventTransformUpdated().AddListener(KtDelegate(this, &KSceneMeshComponent::MarkModelProxyTransformDirty));
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
        Framework.GetRenderer().GetSceneRenderer().RegisterDynamic(&proxy_);
        break;
    }
    case EMobility::Static:
    {
        Framework.GetRenderer().GetSceneRenderer().RegisterStatic(&proxy_);
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
        Framework.GetRenderer().GetSceneRenderer().UnregisterDynamic(&proxy_);
        break;
    }
    case EMobility::Static:
    {
        Framework.GetRenderer().GetSceneRenderer().UnregisterStatic(&proxy_);
        break;
    }
    }
}

void KSceneMeshComponent::Spin()
{
    const float speed = 10.0f * Engine.GetTimeManager().GetDelta();
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
