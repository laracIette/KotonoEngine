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
#include <kotono_platform/Window.h>
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

    spinTask_.duration = 5.0f;
}

void KSceneMeshComponent::Cleanup()
{
    UnregisterModelProxy();

    EventTransformUpdated().RemoveListener(KtDelegate(this, &KSceneMeshComponent::MarkModelProxyTransformDirty));
    Window.GetEventWindowResized().RemoveListener(KtDelegate(this, &KSceneMeshComponent::MarkModelProxyScissorDirty));

    Keyboard.EventKey(EKey::N, EInputState::Pressed)
        .RemoveListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityStatic));
    Keyboard.EventKey(EKey::M, EInputState::Pressed)
        .RemoveListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityDynamic));

    Base::Cleanup();
}

void KSceneMeshComponent::Init()
{
    Base::Init();

    SetCanUpdate(true);

    spinTask_.Start();
}

void KSceneMeshComponent::Update(const float delta)
{
    Base::Update(delta);

    spinTask_.Update(delta);
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

    EventTransformUpdated().AddListener(KtDelegate(this, &KSceneMeshComponent::MarkModelProxyTransformDirty));
    Window.GetEventWindowResized().AddListener(KtDelegate(this, &KSceneMeshComponent::MarkModelProxyScissorDirty));

    Keyboard.EventKey(EKey::N, EInputState::Pressed)
        .AddListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityStatic));
    Keyboard.EventKey(EKey::M, EInputState::Pressed)
        .AddListener(KtDelegate(this, &KSceneMeshComponent::SetMobilityDynamic));
    
    spinTask_.eventUpdate.AddListener(KtDelegate(this, &KSceneMeshComponent::Spin));
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
	modelProxy_ = Renderer.SceneRenderer().CreateProxy();

    modelProxy_->ScheduleUpdate(
        [this](USceneProxy::FrameData& frameData)
        {
            frameData.shader = shader_;
            frameData.renderable = model_;
            frameData.objectData.modelMatrix = ModelMatrix();
            frameData.scissor.offset = GetOwner()->GetViewport()->GetOffset();
            frameData.scissor.extent = GetOwner()->GetViewport()->GetExtent();
        }
    );
}

void KSceneMeshComponent::MarkModelProxyTransformDirty()
{
    modelProxy_->ScheduleUpdate(
        [this](USceneProxy::FrameData& frameData)
        {
            frameData.objectData.modelMatrix = ModelMatrix();
        }
    );
	KT_LOG(ELogImportanceLevel::Medium, "Core.KSceneMeshComponent::MarkModelProxyTransformDirty()", "%s", GetName().c_str());
}

void KSceneMeshComponent::MarkModelProxyScissorDirty()
{
    modelProxy_->ScheduleUpdate(
        [this](USceneProxy::FrameData& frameData)
        {
            frameData.scissor.offset = GetOwner()->GetViewport()->GetOffset();
            frameData.scissor.extent = GetOwner()->GetViewport()->GetExtent();
        }
    );
}

void KSceneMeshComponent::RegisterModelProxy() const
{
    Renderer.SceneRenderer().RegisterProxy(modelProxy_, GetMobility());
}

void KSceneMeshComponent::UnregisterModelProxy() const
{
    Renderer.SceneRenderer().UnregisterProxy(modelProxy_, GetMobility());
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
    KT_LOG(ELogImportanceLevel::High, "Core.KSceneMeshComponent::SetMobilityStatic()", "%s", GetName().c_str());
}

void KSceneMeshComponent::SetMobilityDynamic()
{
    SetMobility(EMobility::Dynamic);
    KT_LOG(ELogImportanceLevel::High, "Core.KSceneMeshComponent::SetMobilityDynamic()", "%s", GetName().c_str());
}
