#include "MeshComponent.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <kotono_common/log.h>
#include <kotono_core/Scene.h>
#include <kotono_graphics/SceneRenderGraph.h>
#include <kotono_input/Keyboard.h>

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

void KMeshComponent::Update(f32 deltaTime)
{
    Base::Update(deltaTime);

    spinTask_.Update(deltaTime);
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
    auto const modelMatrix{ ModelMatrix() };
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

void KMeshComponent::Spin(f32 deltaTime)
{
    f32 const speed{ 10.0f * GetScene()->GetGameTime().lastDelta };
    glm::quat const rotation{ glm::radians(glm::vec3{ 0.0f, speed, 0.0f }) };
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
