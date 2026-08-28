#include "MeshComponent.h"

#include "Task.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <kotono_common/log.h>
#include <kotono_graphics/SceneRenderGraph.h>
#include <kotono_input/Keyboard.h>
#include <kotono_object/Scene.h>

KMeshComponent::KMeshComponent() 
{
    spinTask_.duration = 5.0f;
}

KMeshComponent::~KMeshComponent()
{
    Keyboard.GetEventKey(EKey::N, EInputState::Pressed).RemoveListener(this, &KMeshComponent::SetMobilityStatic);
    Keyboard.GetEventKey(EKey::M, EInputState::Pressed).RemoveListener(this, &KMeshComponent::SetMobilityDynamic);
}

void KMeshComponent::Init()
{
    Base::Init();

    SetCanUpdate(true);

    spinTask_.Start();
}

void KMeshComponent::Update(const f32 deltaTime)
{
    Base::Update(deltaTime);

    spinTask_.Update(deltaTime);
}

const UPath& KMeshComponent::GetShader() const
{
    return shader_;
}

const UPath& KMeshComponent::GetModel() const
{
    return model_;
}

const UPath& KMeshComponent::GetMaterial() const
{
    return material_;
}

void KMeshComponent::SetShader(const UPath& shader)
{
    shader_ = shader;
}

void KMeshComponent::SetModel(const UPath& model)
{
    model_ = model;
}

void KMeshComponent::SetMaterial(const UPath& material)
{
    material_ = material;
}

void KMeshComponent::Spawn()
{
    Base::Spawn();

    Keyboard.GetEventKey(EKey::N, EInputState::Pressed).AddListener(this, &KMeshComponent::SetMobilityStatic);
    Keyboard.GetEventKey(EKey::M, EInputState::Pressed).AddListener(this, &KMeshComponent::SetMobilityDynamic);
    
    spinTask_.eventUpdate.AddListener(this, &KMeshComponent::Spin);
}

void KMeshComponent::PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const
{
    const auto modelMatrix{ ModelMatrix() };
    sceneRenderGraph.drawDatas.push_back({
        .scissor = {},
        .sortKey = {},
        .modelMatrix = modelMatrix,
        .normalMatrix = glm::mat4{ glm::inverseTranspose(glm::mat3{ modelMatrix }) },
        .shader = shader_,
        .material = material_,
        .model = model_,
        .scalars = {},
        .vectors = {},
        .textures = {},
        .isVisible = true,
    });
}

void KMeshComponent::Spin(const f32 deltaTime)
{
    const f32 speed{ 10.0f * GetScene()->GetGameTime().lastDelta };
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
