#include "SceneComponent.h"
#include "SceneObject.h"
#include "Game.h"
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

void KSceneComponent::Update(const float deltaTime)
{
}

const UPtr<TSceneObject>& KSceneComponent::GetOwner() const
{
    return owner_;
}

const UPtr<KSceneComponent>& KSceneComponent::GetParent() const
{
	return parent_;
}

bool KSceneComponent::GetCanUpdate() const
{
    return canUpdate_;
}

const UTransform& KSceneComponent::GetTransform() const
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

bool KSceneComponent::CanSetTransform() const
{
    return mobility_ == EMobility::Dynamic || Game.GetState() == EGameState::Stopped;
}

UEvent<>& KSceneComponent::GetEventTransformUpdated()
{
    return eventTransformUpdated_;
}

void KSceneComponent::SetCanUpdate(const bool canUpdate)
{
    canUpdate_ = canUpdate;
}

void KSceneComponent::SetVisibility(const EVisibility visibility, const bool propagateToChildren)
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

void KSceneComponent::SetMobility(const EMobility mobility)
{
    mobility_ = mobility;
}

const glm::vec3& KSceneComponent::GetRelativePosition() const
{
    return transform_.position;
}

const glm::quat& KSceneComponent::GetRelativeRotation() const
{
    return transform_.rotation;
}

const glm::vec3& KSceneComponent::GetRelativeScale() const
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
    throw std::logic_error("not implemented");
}

void KSceneComponent::SetParent(const UPtr<KSceneComponent>& parent, const ECoordinateSpace keepTransform)
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

void KSceneComponent::SetRelativePosition(const glm::vec3& relativePosition)
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

void KSceneComponent::SetRelativeRotation(const glm::quat& relativeRotation)
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

void KSceneComponent::SetRelativeScale(const glm::vec3& relativeScale)
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

void KSceneComponent::SetWorldPosition(const glm::vec3& worldPosition)
{
    if (parent_)
    {
        SetRelativePosition(worldPosition - parent_->GetWorldPosition());
        return;
    }
    SetRelativePosition(worldPosition);
}

void KSceneComponent::SetWorldRotation(const glm::quat& worldRotation)
{
    if (parent_)
    {
        SetRelativeRotation(glm::inverse(parent_->GetWorldRotation()) * worldRotation);
        return;
    }
    SetRelativeRotation(worldRotation);
}

void KSceneComponent::SetWorldScale(const glm::vec3& worldScale)
{
    if (parent_)
    {
        SetRelativeScale(worldScale / parent_->GetWorldScale());
        return;
    }
    SetRelativeScale(worldScale);
}

void KSceneComponent::Translate(const glm::vec3& offset)
{
    SetRelativePosition(GetRelativePosition() + offset);
}

void KSceneComponent::Rotate(const glm::quat& rotation)
{
    SetRelativeRotation(rotation * GetRelativeRotation());
}

void KSceneComponent::Scale(const glm::vec3& scale)
{
    SetRelativeScale(GetRelativeScale() * scale);
}

glm::vec3 KSceneComponent::GetDirection(const UPtr<const KSceneComponent>& target) const
{
    return target->GetWorldPosition() - GetWorldPosition();
}

float KSceneComponent::GetDistance(const UPtr<const KSceneComponent>& other) const
{
    return glm::distance(GetWorldPosition(), other->GetWorldPosition());
}

void KSceneComponent::Deserialize()
{
	Base::Deserialize();

    for (const auto& sceneComponent : children_)
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

URenderContext* KSceneComponent::GetRenderContext() const
{
    assert(GetOwner());
    return GetOwner()->GetRenderContext();
}

void KSceneComponent::AddChild(const UPtr<KSceneComponent>& component)
{
    if (!component)
    {
        KT_LOG(ELogImportanceLevel::High, "Core", "can't add a null scene component");
        return;
    }

    children_.Add(component);
    component->childrenIndex_ = children_.LastIndex();
    eventTransformUpdated_.AddListener(&component->GetEventTransformUpdated(), &UEvent<>::Broadcast<>);
}

void KSceneComponent::RemoveChild(const UPtr<KSceneComponent>& component)
{
    if (!component)
    {
        KT_LOG(ELogImportanceLevel::High, "Core", "can't add a null scene component");
        return;
    }

    const size index{ component->childrenIndex_ };
    if (children_.RemoveAt(index) == EPoolRemoveResult::ItemSwappedAndRemoved)
    {
        children_[index]->childrenIndex_ = index;
    }
    eventTransformUpdated_.RemoveListener(&component->GetEventTransformUpdated(), &UEvent<>::Broadcast<>);
}

#include "generated/SceneComponent.generated.inl"
