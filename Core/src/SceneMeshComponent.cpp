#include "SceneMeshComponent.h"
#include <kotono_graphics/Renderer.h>
#include <kotono_input/InputManager.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/ModelManager.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Model.h>
#include <kotono_platform/WindowViewport.h>
#include <kotono_common/log.h>
#include <nlohmann/json.hpp>
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
        const auto path{ ::Path.Framework() / R"(shaders\wireframe3D.ktshader)" };
        WireframeShader = ShaderManager.Get(path);
    }
}

void KSceneMeshComponent::Init()
{
    Base::Init();

    spinTask_.duration = 5.0f;
    spinTask_.eventUpdate.AddListener(KtDelegate(this, &KSceneMeshComponent::Spin));
    spinTask_.Start();

    InputManager.Keyboard().EventKey(KT_KEY_N, KT_INPUT_STATE_PRESSED)
        .AddListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityStatic));
    InputManager.Keyboard().EventKey(KT_KEY_M, KT_INPUT_STATE_PRESSED)
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

    InputManager.Keyboard().EventKey(KT_KEY_N, KT_INPUT_STATE_PRESSED)
        .RemoveListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityStatic));
    InputManager.Keyboard().EventKey(KT_KEY_M, KT_INPUT_STATE_PRESSED)
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
//    shader_ = ShaderManager.Get(json["shader"]);
//    model_ = ModelManager.Get(json["model"]);
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
        Renderer.GetSceneRenderer().RegisterDynamic(&modelProxy_);
        break;
    }
    case EMobility::Static:
    {
        Renderer.GetSceneRenderer().RegisterStatic(&modelProxy_);
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
        Renderer.GetSceneRenderer().UnregisterDynamic(&modelProxy_);
        break;
    }
    case EMobility::Static:
    {
        Renderer.GetSceneRenderer().UnregisterStatic(&modelProxy_);
        break;
    }
    }
}

void KSceneMeshComponent::Spin()
{
    const float speed{ 10.0f * TimeManager.GameTime().lastDelta };
    const glm::quat rotation{ glm::quat(glm::radians(glm::vec3(0.0f, speed, 0.0f))) };
    Rotate(rotation);
}

void KSceneMeshComponent::SetMobilityStatic()
{
    SetMobility(EMobility::Static);
    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "Core.KSceneMeshComponent::SetMobilityStatic()", "%s", GetName().c_str());
}

void KSceneMeshComponent::SetMobilityDynamic()
{
    SetMobility(EMobility::Dynamic);
    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "Core.KSceneMeshComponent::SetMobilityDynamic()", "%s", GetName().c_str());
}
