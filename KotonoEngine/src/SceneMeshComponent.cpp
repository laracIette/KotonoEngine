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
    spinTask_ = Engine.ObjectManager().Create<KTask>();

    if (!WireframeShader)
    {
        const auto path = Framework.Path().FrameworkPath() / R"(shaders\wireframe3D.ktshader)";
        WireframeShader = Framework.ShaderManager().Get(path);
    }
}

void KSceneMeshComponent::Init()
{
    Base::Init();

    InitSpin();
    InitModelProxy();

    Framework.InputManager().Keyboard().KeyEvent(KT_KEY_N, KT_INPUT_STATE_PRESSED).AddListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityStatic));
    Framework.InputManager().Keyboard().KeyEvent(KT_KEY_M, KT_INPUT_STATE_PRESSED).AddListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityDynamic));
}

void KSceneMeshComponent::Cleanup()
{
    Base::Cleanup();

    RemoveProxies();
    GetEventTransformUpdated().RemoveListener(KtDelegate(this, &KSceneMeshComponent::MarkModelProxyTransformDirty));

    Framework.InputManager().Keyboard().KeyEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED).RemoveListener(KtDelegate(spinTask_, &KTask::Start));
    spinTask_->Delete();
    
    Framework.InputManager().Keyboard().KeyEvent(KT_KEY_N, KT_INPUT_STATE_PRESSED).RemoveListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityStatic));
    Framework.InputManager().Keyboard().KeyEvent(KT_KEY_M, KT_INPUT_STATE_PRESSED).RemoveListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityDynamic));
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
    json["shader"] = shader_ ? shader_->Path() : "";
    json["model"] = model_ ? model_->Path() : "";
}

void KSceneMeshComponent::DeserializeFrom(const nlohmann::json& json)
{
    Base::DeserializeFrom(json);
    shader_ = Framework.ShaderManager().Get(json["shader"]);
    model_ = Framework.ModelManager().Get(json["model"]);
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
    Framework.InputManager().Keyboard().KeyEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED).AddListener(KtDelegate(spinTask_, &KTask::Start));
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
    proxy_.viewport = Owner()->GetViewport();
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
        Framework.Renderer().GetSceneRenderer().RegisterDynamic(&proxy_);
        break;
    }
    case EMobility::Static:
    {
        Framework.Renderer().GetSceneRenderer().RegisterStatic(&proxy_);
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
        Framework.Renderer().GetSceneRenderer().UnregisterDynamic(&proxy_);
        break;
    }
    case EMobility::Static:
    {
        Framework.Renderer().GetSceneRenderer().UnregisterStatic(&proxy_);
        break;
    }
    }
}

void KSceneMeshComponent::Spin()
{
    const float speed = 10.0f * Engine.TimeManager().GetDelta();
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
