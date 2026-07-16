#include "MeshComponent.h"
#include "SceneObject.h"
#include "Task.h"
#include "TimeManager.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_graphics/DrawCall.h>
#include <kotono_graphics/DrawDataBufferData.h>
#include <kotono_graphics/Material.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/TransformBufferData.h>
#include <kotono_input/Keyboard.h>
#include <kotono_platform/Window.h>
#include <kotono_platform/WindowViewport.h>

static UAsset<UShader> WireframeShader;

KMeshComponent::KMeshComponent() 
    : drawCallBuilder_{}
{
    if (!WireframeShader)
    {
        //WireframeShader = SAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/wireframe3D.kasset");
    }

    spinTask_.duration = 5.0f;
}

KMeshComponent::~KMeshComponent()
{
}

void KMeshComponent::Init()
{
    Base::Init();

    SetCanUpdate(true);

    spinTask_.Start();
}

void KMeshComponent::Update(const float deltaTime)
{
    Base::Update(deltaTime);

    spinTask_.Update(deltaTime);
}

const UAsset<UShader>& KMeshComponent::GetShader() const
{
    return shader_;
}

const UAsset<UModel>& KMeshComponent::GetModel() const
{
    return model_;
}

const UAsset<UMaterial>& KMeshComponent::GetMaterial() const
{
    return material_;
}

void KMeshComponent::SetShader(const UAsset<UShader>& shader)
{
    shader_ = shader;
    RefreshDrawCallShaderData();
}

void KMeshComponent::SetModel(const UAsset<UModel>& model)
{
    model_ = model;
    RefreshDrawCallModelData();
}

void KMeshComponent::SetMaterial(const UAsset<UMaterial>& material)
{
    material_ = material;
    RefreshDrawCallMaterialData();
}

void KMeshComponent::Spawn()
{
    Base::Spawn();

    RefreshDrawCall();
    RegisterDrawCall();

    GetEventTransformUpdated().AddListener(this, &KMeshComponent::RefreshDrawCallTransformData);
    RegisterDelegate(&Window, Window.GetEventWindowResized(), this, &KMeshComponent::RefreshDrawCallScissor);

    RegisterDelegate(&Keyboard, Keyboard.EventKey(EKey::N, EInputState::Pressed), this, &KMeshComponent::SetMobilityStatic);
    RegisterDelegate(&Keyboard, Keyboard.EventKey(EKey::M, EInputState::Pressed), this, &KMeshComponent::SetMobilityDynamic);
    
    spinTask_.eventUpdate.AddListener(this, &KMeshComponent::Spin);
}

void KMeshComponent::SetVisibility(const EVisibility visibility, const bool propagateToChildren)
{
    Base::SetVisibility(visibility, propagateToChildren);
}

void KMeshComponent::SetMobility(const EMobility mobility)
{
    Base::SetMobility(mobility);
}

void KMeshComponent::RegisterDrawCall()
{
    drawCallBuilder_.Register(ERenderBucket::Opaque);
}

void KMeshComponent::UnregisterDrawCall()
{
    drawCallBuilder_.Unregister();
}

void KMeshComponent::RefreshDrawCall() const
{
    RefreshDrawCallScissor();
    RefreshDrawCallShaderData();
    RefreshDrawCallModelData();
    RefreshDrawCallMaterialData();
    RefreshDrawCallTransformData();
}

void KMeshComponent::RefreshDrawCallScissor() const
{
    const auto& offset{ GetOwner()->GetViewport()->GetOffset() };
    const auto& extent{ GetOwner()->GetViewport()->GetExtent() };

    drawCallBuilder_.GetDrawCall()->scissor = {
        .offset = { offset.x, offset.y },
        .extent = { extent.x, extent.y },
    };
}

void KMeshComponent::RefreshDrawCallShaderData() const
{
    if (shader_)
    {
        drawCallBuilder_.GetDrawCall()->pipeline = shader_->GetPipeline();
    }
}

void KMeshComponent::RefreshDrawCallModelData() const
{
    if (model_)
    {
        drawCallBuilder_.GetDrawData()->vertexBufferAddress = model_->GetVertexBufferAddress();
        drawCallBuilder_.GetDrawCall()->indexCount = model_->GetIndexCount();
        drawCallBuilder_.GetDrawCall()->firstIndex = model_->GetFirstIndex();
    }
}

void KMeshComponent::RefreshDrawCallMaterialData() const
{
    if (material_)
    {
        drawCallBuilder_.GetDrawData()->materialIndex = material_->GetIndex();
    }
}

void KMeshComponent::RefreshDrawCallTransformData() const
{
    const auto modelMatrix{ ModelMatrix() };
    drawCallBuilder_.GetTransform()->modelMatrix = modelMatrix;
    drawCallBuilder_.GetTransform()->normalMatrix = glm::mat4{ glm::inverseTranspose(glm::mat3{ modelMatrix }) };
}

void KMeshComponent::Spin()
{
    const float speed{ 10.0f * TimeManager.GameTime().lastDelta };
    const glm::quat rotation{ glm::radians(glm::vec3{ 0.0f, speed, 0.0f }) };
    Rotate(rotation);
}

void KMeshComponent::SetMobilityStatic()
{
    SetMobility(EMobility::Static);
    KT_LOG(ELogImportanceLevel::High, "Core", "{0}", GetName());
}

void KMeshComponent::SetMobilityDynamic()
{
    SetMobility(EMobility::Dynamic);
    KT_LOG(ELogImportanceLevel::High, "Core", "{0}", GetName());
}

#include "generated/MeshComponent.generated.inl"
