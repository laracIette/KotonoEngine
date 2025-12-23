#include "ObjectManager.h"
#include "SceneMeshComponent.h"
#include "SceneObject.h"
#include "Task.h"
#include "TimeManager.h"
#include <kotono_common/log.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/ModelManager.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_input/Keyboard.h>
#include <kotono_platform/WindowViewport.h>
#include <nlohmann/json.hpp>

static KtShader* WireframeShader = nullptr;

KSceneMeshComponent::KSceneMeshComponent(UPtrOwnerBase* ptrOwner) :
    Base(ptrOwner)
{
    if (!WireframeShader)
    {
        const auto path{ KtPath::Graphics() / "shaders" / "wireframe3D.ktshader" };
        WireframeShader = ShaderManager.Get(path);
    }
}

void KSceneMeshComponent::Init()
{
    Base::Init();

    EventTransformUpdated().AddListener(KtDelegate(this, &KSceneMeshComponent::MarkModelProxyTransformDirty));

    Keyboard.EventKey(KT_KEY_N, KT_INPUT_STATE_PRESSED)
        .AddListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityStatic));
    Keyboard.EventKey(KT_KEY_M, KT_INPUT_STATE_PRESSED)
        .AddListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityDynamic));
}

void KSceneMeshComponent::Update()
{
    Base::Update();

    spinTask_.Update();
}

void KSceneMeshComponent::Cleanup()
{
    UnregisterModelProxy();
    EventTransformUpdated().RemoveListener(KtDelegate(this, &KSceneMeshComponent::MarkModelProxyTransformDirty));

    Keyboard.EventKey(KT_KEY_N, KT_INPUT_STATE_PRESSED)
        .RemoveListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityStatic));
    Keyboard.EventKey(KT_KEY_M, KT_INPUT_STATE_PRESSED)
        .RemoveListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityDynamic));

    Base::Cleanup();
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

void KSceneMeshComponent::Spawn()
{
    Base::Spawn();

    CreateModelProxy();
    RegisterModelProxy();

    spinTask_.duration = 5.0f;
    spinTask_.eventUpdate.AddListener(KtDelegate(this, &KSceneMeshComponent::Spin));
    spinTask_.Start();
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
    modelProxy_.scissor.offset = GetOwner()->GetViewport()->GetOffset();
    modelProxy_.scissor.extent = GetOwner()->GetViewport()->GetExtent();
#   ifdef _DEBUG
        modelProxy_.source = this;
#   endif
}

void KSceneMeshComponent::MarkModelProxyTransformDirty()
{
    modelProxy_.MarkDirty();
    modelProxy_.objectData.modelMatrix = ModelMatrix();
	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_LOW, "Core.KSceneMeshComponent::MarkModelProxyTransformDirty()", "%s", GetName().c_str());
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
