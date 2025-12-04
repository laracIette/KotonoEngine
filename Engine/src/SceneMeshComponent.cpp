#include "SceneMeshComponent.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ModelManager.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/Model.h>
#include <kotono_framework/WindowViewport.h>
#include <kotono_framework/Clock.h>
#include "log.h"
#include <nlohmann/json.hpp>
#include "Engine.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "Task.h"

static KtShader* WireframeShader = nullptr;

KSceneMeshComponent::KSceneMeshComponent(UPtrOwnerBase* ptrOwner, const UPtr<TSceneObject>& owner) :
    Base(ptrOwner, owner)
{
    if (!WireframeShader)
    {
        const auto path{ Framework.Path().Framework() / R"(shaders\wireframe3D.ktshader)" };
        WireframeShader = Framework.ShaderManager().Get(path);
    }
}

void KSceneMeshComponent::Init()
{
    Base::Init();

    spinTask_.duration = 5.0f;
    spinTask_.eventUpdate.AddListener(KtDelegate(this, &KSceneMeshComponent::Spin));
    spinTask_.Start();

    Framework.InputManager().Keyboard().EventKey(KT_KEY_N, KT_INPUT_STATE_PRESSED)
        .AddListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityStatic));
    Framework.InputManager().Keyboard().EventKey(KT_KEY_M, KT_INPUT_STATE_PRESSED)
        .AddListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityDynamic));
}

void KSceneMeshComponent::Update()
{
    Base::Update();

    spinTask_.Update();
}

void KSceneMeshComponent::Cleanup()
{
    Base::Cleanup();

    UnregisterModelProxy();
    EventTransformUpdated().RemoveListener(KtDelegate(this, &KSceneMeshComponent::MarkModelProxyTransformDirty));

    Framework.InputManager().Keyboard().EventKey(KT_KEY_N, KT_INPUT_STATE_PRESSED)
        .RemoveListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityStatic));
    Framework.InputManager().Keyboard().EventKey(KT_KEY_M, KT_INPUT_STATE_PRESSED)
        .RemoveListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityDynamic));
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

//void KSceneMeshComponent::SerializeTo(nlohmann::json& json) const
//{
//    Base::SerializeTo(json);
//    json["shader"] = shader_ ? shader_->Path() : "";
//    json["model"] = model_ ? model_->Path() : "";
//}

//void KSceneMeshComponent::DeserializeFrom(const nlohmann::json& json)
//{
//    Base::DeserializeFrom(json);
//    shader_ = Framework.ShaderManager().Get(json["shader"]);
//    model_ = Framework.ModelManager().Get(json["model"]);
//}

void KSceneMeshComponent::Spawn()
{
    Base::Spawn();

    CreateModelProxy();
    RegisterModelProxy();
    EventTransformUpdated().AddListener(KtDelegate(this, &KSceneMeshComponent::MarkModelProxyTransformDirty));
}

void KSceneMeshComponent::SetVisibility(const EVisibility visibility, const bool propagateToChildren)
{
    UnregisterModelProxy();
    Base::SetVisibility(visibility, propagateToChildren);
    RegisterModelProxy();
}

void KSceneMeshComponent::SetMobility(const EMobility mobility)
{
    UnregisterModelProxy();
    Base::SetMobility(mobility);
    RegisterModelProxy();
}

void KSceneMeshComponent::CreateModelProxy()
{
    modelProxy_.shader = shader_;
    modelProxy_.renderable = model_;
    modelProxy_.objectData.modelMatrix = ModelMatrix();
    modelProxy_.scissor.offset = Owner()->GetViewport()->GetOffset();
    modelProxy_.scissor.extent = Owner()->GetViewport()->GetExtent();
#   ifdef _DEBUG
        modelProxy_.source = this;
#   endif
}

void KSceneMeshComponent::MarkModelProxyTransformDirty()
{
    modelProxy_.isDirty = true;
    modelProxy_.objectData.modelMatrix = ModelMatrix();
}

void KSceneMeshComponent::RegisterModelProxy()
{
    switch (GetMobility())
    {
    case EMobility::Dynamic:
    {
        Framework.Renderer().GetSceneRenderer().RegisterDynamic(&modelProxy_);
        break;
    }
    case EMobility::Static:
    {
        Framework.Renderer().GetSceneRenderer().RegisterStatic(&modelProxy_);
        break;
    }
    }
}

void KSceneMeshComponent::UnregisterModelProxy()
{
    switch (GetMobility())
    {
    case EMobility::Dynamic:
    {
        Framework.Renderer().GetSceneRenderer().UnregisterDynamic(&modelProxy_);
        break;
    }
    case EMobility::Static:
    {
        Framework.Renderer().GetSceneRenderer().UnregisterStatic(&modelProxy_);
        break;
    }
    }
}

void KSceneMeshComponent::Spin()
{
    const float speed{ 10.0f * Engine.TimeManager().GameTime().lastDelta };
    const glm::quat rotation{ glm::quat(glm::radians(glm::vec3(0.0f, speed, 0.0f))) };
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
