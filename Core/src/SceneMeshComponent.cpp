#include "SceneMeshComponent.h"
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

KSceneMeshComponent::KSceneMeshComponent() 
    : drawCallBuilder_{}
{
    if (!WireframeShader)
    {
        //WireframeShader = SAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/wireframe3D.kasset");
    }

    spinTask_.duration = 5.0f;
}

KSceneMeshComponent::~KSceneMeshComponent()
{
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

    RefreshDrawCall();
    RegisterDrawCall();

    EventTransformUpdated().AddListener(this, &KSceneMeshComponent::RefreshDrawCallTransformData);
    RegisterDelegate(&Window, Window.GetEventWindowResized(), this, &KSceneMeshComponent::RefreshDrawCallScissor);

    RegisterDelegate(&Keyboard, Keyboard.EventKey(EKey::N, EInputState::Pressed), this, &KSceneMeshComponent::SetMobilityStatic);
    RegisterDelegate(&Keyboard, Keyboard.EventKey(EKey::M, EInputState::Pressed), this, &KSceneMeshComponent::SetMobilityDynamic);
    
    spinTask_.eventUpdate.AddListener(this, &KSceneMeshComponent::Spin);
}

void KSceneMeshComponent::SetVisibility(const EVisibility visibility, const bool propagateToChildren)
{
    Base::SetVisibility(visibility, propagateToChildren);
}

void KSceneMeshComponent::SetMobility(const EMobility mobility)
{
    Base::SetMobility(mobility);
}

void KSceneMeshComponent::RegisterDrawCall()
{
    drawCallBuilder_.Register(ERenderBucket::Opaque);
}

void KSceneMeshComponent::UnregisterDrawCall()
{
    drawCallBuilder_.Unregister();
}

void KSceneMeshComponent::RefreshDrawCall() const
{
    RefreshDrawCallScissor();
    RefreshDrawCallShaderData();
    RefreshDrawCallModelData();
    RefreshDrawCallMaterialData();
    RefreshDrawCallTransformData();
}

void KSceneMeshComponent::RefreshDrawCallScissor() const
{
    const auto& offset{ GetOwner()->GetViewport()->GetOffset() };
    const auto& extent{ GetOwner()->GetViewport()->GetExtent() };

    drawCallBuilder_.GetDrawCall()->scissor = {
        .offset = { offset.x, offset.y },
        .extent = { extent.x, extent.y },
    };
}

void KSceneMeshComponent::RefreshDrawCallShaderData() const
{
    if (shader_)
    {
        drawCallBuilder_.GetDrawCall()->pipeline = shader_->GetPipeline();
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
    const auto modelMatrix{ ModelMatrix() };
    drawCallBuilder_.GetTransform()->modelMatrix = modelMatrix;
    drawCallBuilder_.GetTransform()->normalMatrix = glm::mat4{ glm::inverseTranspose(glm::mat3{ modelMatrix }) };
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
