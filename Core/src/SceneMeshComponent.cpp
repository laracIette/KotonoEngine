#include "SceneMeshComponent.h"
#include "SceneObject.h"
#include "Task.h"
#include "TimeManager.h"
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_graphics/DrawCall.h>
#include <kotono_graphics/DrawDataBuffer.h>
#include <kotono_graphics/Material.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/ParametersBuffer.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/SceneProxy.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/TransformBuffer.h>
#include <kotono_graphics/Color.h>
#include <kotono_input/Keyboard.h>
#include <kotono_platform/Window.h>
#include <kotono_platform/WindowViewport.h>

static UAsset<UShader> WireframeShader;

KSceneMeshComponent::KSceneMeshComponent() 
    : modelProxy_{ new USceneProxy{} }
    , drawCallBuilder_{}
{
    if (!WireframeShader)
    {
        //WireframeShader = UAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/shaders/wireframe3D.ktshader");
    }

    spinTask_.duration = 5.0f;
}

KSceneMeshComponent::~KSceneMeshComponent()
{
    UnregisterModelProxy();
    Renderer.SceneRenderer().DeleteProxy(modelProxy_);
    UnregisterDrawCall();
}

void KSceneMeshComponent::Init()
{
    Base::Init();

    SetCanUpdate(true);

    spinTask_.Start();
}

void KSceneMeshComponent::Update(const float deltaTime)
{
    Base::Update(deltaTime);

    spinTask_.Update(deltaTime);
}

const UAsset<UShader>& KSceneMeshComponent::GetShader() const
{
    return shader_;
}

const UAsset<UModel>& KSceneMeshComponent::GetModel() const
{
    return model_;
}

const UAsset<UMaterial>& KSceneMeshComponent::GetMaterial() const
{
    return material_;
}

void KSceneMeshComponent::SetShader(const UAsset<UShader>& shader)
{
    shader_ = shader;
    RefreshDrawCallShaderData();
}

void KSceneMeshComponent::SetModel(const UAsset<UModel>& model)
{
    model_ = model;
    RefreshDrawCallModelData();
}

void KSceneMeshComponent::SetMaterial(const UAsset<UMaterial>& material)
{
    material_ = material;
    RefreshDrawCallMaterialData();
}

void KSceneMeshComponent::Spawn()
{
    Base::Spawn();

    CreateModelProxy();
    RegisterModelProxy();
    RefreshDrawCall();
    RegisterDrawCall();

    EventTransformUpdated().AddListener(this, &KSceneMeshComponent::MarkModelProxyTransformDirty);
    
    RegisterDelegate(&Window, Window.GetEventWindowResized(), this, &KSceneMeshComponent::MarkModelProxyScissorDirty);

    RegisterDelegate(&Keyboard, Keyboard.EventKey(EKey::N, EInputState::Pressed), this, &KSceneMeshComponent::SetMobilityStatic);
    RegisterDelegate(&Keyboard, Keyboard.EventKey(EKey::M, EInputState::Pressed), this, &KSceneMeshComponent::SetMobilityDynamic);
    
    spinTask_.eventUpdate.AddListener(this, &KSceneMeshComponent::Spin);
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
    modelProxy_->ScheduleUpdate(
        [this](USceneProxy::Data& data)
        {
            data.shader = shader_.Get();
            data.renderable = model_.Get();
            data.objectData.modelMatrix = ModelMatrix();
            data.scissor.offset = GetOwner()->GetViewport()->GetOffset();
            data.scissor.extent = GetOwner()->GetViewport()->GetExtent();
        }
    );
}

void KSceneMeshComponent::MarkModelProxyTransformDirty()
{
    RefreshDrawCallTransformData();

    modelProxy_->ScheduleUpdate(
        [this](USceneProxy::Data& data)
        {
            data.objectData.modelMatrix = ModelMatrix();
        }
    );
	KT_LOG(ELogImportanceLevel::Medium, "Core", "{0}", GetName());
}

void KSceneMeshComponent::MarkModelProxyScissorDirty()
{
    modelProxy_->ScheduleUpdate(
        [this](USceneProxy::Data& data)
        {
            data.scissor.offset = GetOwner()->GetViewport()->GetOffset();
            data.scissor.extent = GetOwner()->GetViewport()->GetExtent();
        }
    );
}

void KSceneMeshComponent::RegisterModelProxy() const
{
    Renderer.SceneRenderer().RegisterProxy(modelProxy_, GetMobility());
}

void KSceneMeshComponent::RegisterDrawCall()
{
    drawCallBuilder_.Register();
}

void KSceneMeshComponent::UnregisterModelProxy() const
{
    Renderer.SceneRenderer().UnregisterProxy(modelProxy_, GetMobility());
}

void KSceneMeshComponent::UnregisterDrawCall()
{
    drawCallBuilder_.Unregister();
}

void KSceneMeshComponent::RefreshDrawCall() const
{
    drawCallBuilder_.GetDrawCall()->renderBucket = ERenderBucket::Opaque;

    const auto& offset{ GetOwner()->GetViewport()->GetOffset() };
    const auto& extent{ GetOwner()->GetViewport()->GetExtent() };

    drawCallBuilder_.GetDrawCall()->scissor = {
        .offset = { offset.x, offset.y },
        .extent = { extent.x, extent.y },
    };
    
    RefreshDrawCallShaderData();
    RefreshDrawCallModelData();
    RefreshDrawCallMaterialData();
    RefreshDrawCallTransformData();
}

void KSceneMeshComponent::RefreshDrawCallShaderData() const
{
    if (shader_)
    {
        drawCallBuilder_.GetDrawCall()->pipeline = shader_->GetGraphicsPipeline();
    }
}

void KSceneMeshComponent::RefreshDrawCallModelData() const
{
    if (model_)
    {
        drawCallBuilder_.GetDrawCall()->vertexBufferAdress = model_->GetVertexBufferAddress();
        drawCallBuilder_.GetDrawCall()->indexBuffer = model_->GetIndexBuffer();
        drawCallBuilder_.GetDrawCall()->indexCount = model_->GetIndexCount();
        drawCallBuilder_.GetDrawCall()->firstIndex = 0;
    }
}

void KSceneMeshComponent::RefreshDrawCallMaterialData() const
{
    if (material_)
    {
        drawCallBuilder_.GetDrawData()->materialIndex = material_->GetIndex();
    }
}

void KSceneMeshComponent::RefreshDrawCallTransformData() const
{
    drawCallBuilder_.GetTransform()->modelMatrix = ModelMatrix();
    drawCallBuilder_.GetTransform()->normalMatrix = glm::identity<glm::mat4>();
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
    KT_LOG(ELogImportanceLevel::High, "Core", "{0}", GetName());
}

void KSceneMeshComponent::SetMobilityDynamic()
{
    SetMobility(EMobility::Dynamic);
    KT_LOG(ELogImportanceLevel::High, "Core", "{}", GetName());
}

#include "generated/SceneMeshComponent.generated.inl"
