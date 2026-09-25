#include "MeshComponent.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <kotono_common/enum_utils.h>
#include <kotono_common/log.h>
#include <kotono_graphics/SceneRenderGraph.h>
#include <kotono_graphics/SceneVisibility.h>

KMeshComponent::KMeshComponent()
{
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

void KMeshComponent::Update(f32 deltaTime)
{
    Base::Update(deltaTime);

    spinTask_.Update(deltaTime);
}

void KMeshComponent::Spawn()
{
    Base::Spawn();

    spinTask_.eventUpdate.AddListener(this, &KMeshComponent::Spin);
}

void KMeshComponent::Despawn()
{
    Base::Spawn();

    spinTask_.eventUpdate.RemoveListener(this, &KMeshComponent::Spin);
}

void KMeshComponent::PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph, ESceneVisibility visibility) const
{
    if (has_flag(visibility, ESceneVisibility::Mesh))
    {
        auto const modelMatrix{ ModelMatrix() };
        sceneRenderGraph.drawDatas.push_back({
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
}

void KMeshComponent::Spin(f32 deltaTime)
{
    f32 const speed{ 10.0f * deltaTime };
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
