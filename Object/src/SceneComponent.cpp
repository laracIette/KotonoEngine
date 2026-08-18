#include "SceneComponent.h"

#include "SceneObject.h"
#include <glm/gtc/quaternion.hpp>
#include <kotono_common/log.h>
#include <kotono_platform/glm_utils.h>
#include <stdexcept>

KSceneComponent::KSceneComponent() 
    : owner_{}
    , visibility_{ EVisibility::Visible }
    , canUpdate_{ true }
    , isInit_{ false }
    , mobility_{ EMobility::Dynamic }
{
}

KSceneComponent::~KSceneComponent()
{
    if (owner_)
    {
        owner_->RemoveComponent(Ptr());
    }
    if (parent_)
    {
        parent_->RemoveChild(Ptr());
    }
}

void KSceneComponent::Init()
{
}

void KSceneComponent::Update(f32 deltaTime)
{
}

UPtr<TSceneObject> const& KSceneComponent::GetOwner() const
{
    return owner_;
}

UScene* KSceneComponent::GetScene() const
{
    assert(GetOwner());
    return GetOwner()->GetScene();
}

UPtr<KSceneComponent> const& KSceneComponent::GetParent() const
{
	return parent_;
}

b8 KSceneComponent::GetCanUpdate() const
{
    return canUpdate_;
}

UTransform const& KSceneComponent::GetTransform() const
{
    return transform_;
}

EVisibility KSceneComponent::GetVisibility() const
{
    return visibility_;
}

EMobility KSceneComponent::GetMobility() const
{
    return mobility_;
}

b8 KSceneComponent::CanSetTransform() const
{
    return mobility_ == EMobility::Dynamic;
}

UEvent<>& KSceneComponent::GetEventTransformUpdated()
{
    return eventTransformUpdated_;
}

void KSceneComponent::SetCanUpdate(b8 canUpdate)
{
    canUpdate_ = canUpdate;
}

void KSceneComponent::SetVisibility(EVisibility visibility, b8 propagateToChildren)
{
    visibility_ = visibility;
    if (propagateToChildren)
    {
        for (auto& sceneComponent : children_)
        {
            sceneComponent->SetVisibility(visibility, propagateToChildren);
        }
    }
}

void KSceneComponent::SetMobility(EMobility mobility)
{
    mobility_ = mobility;
}

glm::vec3 const& KSceneComponent::GetRelativePosition() const
{
    return transform_.position;
}

glm::quat const& KSceneComponent::GetRelativeRotation() const
{
    return transform_.rotation;
}

glm::vec3 const& KSceneComponent::GetRelativeScale() const
{
    return transform_.scale;
}

glm::vec3 KSceneComponent::GetWorldPosition() const
{
    if (parent_)
    {
        return parent_->GetWorldPosition() + parent_->GetWorldRotation() * (parent_->GetWorldScale() * GetRelativePosition());
    }
    return GetRelativePosition();
}

glm::quat KSceneComponent::GetWorldRotation() const
{
    if (parent_)
    {
        return glm::normalize(parent_->GetWorldRotation() * GetRelativeRotation());
    }
    return GetRelativeRotation();
}

glm::vec3 KSceneComponent::GetWorldScale() const
{
    if (parent_)
    {
        return parent_->GetWorldScale() * GetRelativeScale();
    }
    return GetRelativeScale();
}

glm::vec3 KSceneComponent::RightVector() const
{
    return GetWorldRotation() * WorldRightVector;
}

glm::vec3 KSceneComponent::UpVector() const
{
    return GetWorldRotation() * WorldUpVector;
}

glm::vec3 KSceneComponent::ForwardVector() const
{
    return GetWorldRotation() * WorldForwardVector;
}

glm::mat4 KSceneComponent::TranslationMatrix() const
{
    return glm::translate(glm::identity<glm::mat4>(), GetWorldPosition());
}

glm::mat4 KSceneComponent::RotationMatrix() const
{
    return glm::mat4_cast(GetWorldRotation());
}

glm::mat4 KSceneComponent::ScaleMatrix() const
{
    return glm::scale(glm::identity<glm::mat4>(), GetWorldScale());
}

glm::mat4 KSceneComponent::ModelMatrix() const
{
    return TranslationMatrix() * RotationMatrix() * ScaleMatrix();
}

glm::vec3 KSceneComponent::GetScreenPosition() const
{
    throw std::runtime_error("not implemented");
}

void KSceneComponent::SetParent(UPtr<KSceneComponent> const& parent, ECoordinateSpace keepTransform)
{
    if (!parent && !parent_)
    {
        KT_LOG(ELogImportanceLevel::High, "Core", "couldn't set the parent of {0}, it's already null", GetName());
        return;
    }

    if (parent == this)
    {
        KT_LOG(ELogImportanceLevel::High, "Core", "couldn't set the parent of {0} to itself", GetName());
        return;
    }

    if (parent == parent_)
    {
        KT_LOG(ELogImportanceLevel::High, "Core", "couldn't set the parent of {0} to its current parent", GetName());
        return;
    }

    if (!CanSetTransform())
    {
        KT_LOG(ELogImportanceLevel::High, "Core", "couldn't set the parent of {0}, its mobility is static", GetName());
        return;
    }

    if (parent_)
    {
        parent_->RemoveChild(Ptr());
    }

    switch (keepTransform)
    {
    case ECoordinateSpace::Relative:
    {
        parent_ = parent;
        break;
    }
    case ECoordinateSpace::World:
    {
        const auto position{ GetWorldPosition() };
        const auto rotation{ GetWorldRotation() };
        const auto scale{ GetWorldScale() };
        parent_ = parent;
        SetWorldPosition(position);
        SetWorldRotation(rotation);
        SetWorldScale(scale);
        break;
    }
    }

    if (parent_)
    {
        parent_->AddChild(Ptr());
    }

    owner_ = parent_ ? parent_->owner_ : nullptr;
}

void KSceneComponent::SetRelativePosition(glm::vec3 const& relativePosition)
{
    if (!CanSetTransform())
    {
        KT_LOG(ELogImportanceLevel::High, "Core", "couldn't set the position of {}, its mobility is static", GetName());
        return;
    }

    if (transform_.position == relativePosition)
    {
        return;
    }

    transform_.position = relativePosition;
    eventTransformUpdated_.Broadcast();
}

void KSceneComponent::SetRelativeRotation(glm::quat const& relativeRotation)
{
    if (!CanSetTransform())
    {
        KT_LOG(ELogImportanceLevel::High, "Core", "couldn't set the rotation of {}, its mobility is static", GetName());
        return;
    }

    if (transform_.rotation == relativeRotation)
    {
        return;
    }

    transform_.rotation = glm::normalize(relativeRotation);
    eventTransformUpdated_.Broadcast();
}

void KSceneComponent::SetRelativeScale(glm::vec3 const& relativeScale)
{
    if (!CanSetTransform())
    {
        KT_LOG(ELogImportanceLevel::High, "Core", "couldn't set the scale of {}, its mobility is static", GetName());
        return;
    }

    if (transform_.scale == relativeScale)
    {
        return;
    }

    transform_.scale = relativeScale;
    eventTransformUpdated_.Broadcast();
}

void KSceneComponent::SetWorldPosition(glm::vec3 const& worldPosition)
{
    if (parent_)
    {
        SetRelativePosition(worldPosition - parent_->GetWorldPosition());
        return;
    }
    SetRelativePosition(worldPosition);
}

void KSceneComponent::SetWorldRotation(glm::quat const& worldRotation)
{
    if (parent_)
    {
        SetRelativeRotation(glm::inverse(parent_->GetWorldRotation()) * worldRotation);
        return;
    }
    SetRelativeRotation(worldRotation);
}

void KSceneComponent::SetWorldScale(glm::vec3 const& worldScale)
{
    if (parent_)
    {
        SetRelativeScale(worldScale / parent_->GetWorldScale());
        return;
    }
    SetRelativeScale(worldScale);
}

void KSceneComponent::Translate(glm::vec3 const& offset)
{
    SetRelativePosition(GetRelativePosition() + offset);
}

void KSceneComponent::Rotate(glm::quat const& rotation)
{
    SetRelativeRotation(rotation * GetRelativeRotation());
}

void KSceneComponent::Scale(glm::vec3 const& scale)
{
    SetRelativeScale(GetRelativeScale() * scale);
}

glm::vec3 KSceneComponent::GetDirection(UPtr<KSceneComponent const> const& target) const
{
    return target->GetWorldPosition() - GetWorldPosition();
}

float KSceneComponent::GetDistance(UPtr<KSceneComponent const> const& other) const
{
    return glm::distance(GetWorldPosition(), other->GetWorldPosition());
}

void KSceneComponent::Deserialize()
{
	Base::Deserialize();

    for (auto& sceneComponent : children_)
    {
        if (sceneComponent)
        {
            sceneComponent->parent_ = Ptr();
            eventTransformUpdated_.AddListener(&sceneComponent->GetEventTransformUpdated(), &UEvent<>::Broadcast<>);
        }
    }
}

void KSceneComponent::Spawn()
{
}

void KSceneComponent::PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const
{
}

void KSceneComponent::AddChild(UPtr<KSceneComponent> const& component)
{
    if (!component)
    {
        KT_LOG(ELogImportanceLevel::High, "Core", "can't add a null scene component");
        return;
    }

    children_.Add(component);
    eventTransformUpdated_.AddListener(&component->GetEventTransformUpdated(), &UEvent<>::Broadcast<>);
}

void KSceneComponent::RemoveChild(UPtr<KSceneComponent> const& component)
{
    if (!component)
    {
        KT_LOG(ELogImportanceLevel::High, "Core", "can't add a null scene component");
        return;
    }

    children_.Remove(component);
    eventTransformUpdated_.RemoveListener(&component->GetEventTransformUpdated(), &UEvent<>::Broadcast<>);
}

#include "generated/SceneComponent.generated.inl"
