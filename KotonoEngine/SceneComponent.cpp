#include "SceneComponent.h"
#include "SceneObject.h"
#include <stdexcept>

KSceneComponent::KSceneComponent(TSceneObject* owner) :
    owner_(owner),
    parent_(nullptr)
{
}

void KSceneComponent::Init()
{
    Base::Init();

    if (owner_->GetRootComponent() != this)
    {
        SetParent(owner_->GetRootComponent(), ECoordinateSpace::Relative);
    }
}

TSceneObject* KSceneComponent::GetOwner() const
{
    return owner_;
}

const UTransform& KSceneComponent::GetTransform() const
{
    return transform_;
}

const EVisibility KSceneComponent::GetVisibility() const
{
    return visibility_;
}

KtEvent<>& KSceneComponent::GetEventUpdateTransform()
{
    return eventUpdateTransform_;
}

void KSceneComponent::SetVisibility(const EVisibility visibility)
{
    visibility_ = visibility;
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

const glm::vec3 KSceneComponent::GetWorldPosition() const
{
    if (parent_)
    {
        return parent_->GetWorldPosition() + GetRelativePosition();
    }
    return GetRelativePosition();
}

const glm::quat KSceneComponent::GetWorldRotation() const
{
    if (parent_)
    {
        return glm::normalize(parent_->GetWorldRotation() * GetRelativeRotation());
    }
    return GetRelativeRotation();
}

const glm::vec3 KSceneComponent::GetWorldScale() const
{
    if (parent_)
    {
        return parent_->GetWorldScale() * GetRelativeScale();
    }
    return GetRelativeScale();
}

const glm::vec3 KSceneComponent::GetRightVector() const
{
    return GetWorldRotation() * glm::vec3(1.0f, 0.0f, 0.0f);
}

const glm::vec3 KSceneComponent::GetForwardVector() const
{
    return GetWorldRotation() * glm::vec3(0.0f, 1.0f, 0.0f);
}

const glm::vec3 KSceneComponent::GetUpVector() const
{
    return GetWorldRotation() * glm::vec3(0.0f, 0.0f, 1.0f);
}

const glm::mat4 KSceneComponent::GetTranslationMatrix() const
{
    return glm::translate(glm::identity<glm::mat4>(), GetWorldPosition());
}

const glm::mat4 KSceneComponent::GetRotationMatrix() const
{
    return glm::mat4_cast(GetWorldRotation());
}

const glm::mat4 KSceneComponent::GetScaleMatrix() const
{
    return glm::scale(glm::identity<glm::mat4>(), GetWorldScale());
}

const glm::mat4 KSceneComponent::GetModelMatrix() const
{
    return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
}

const glm::vec3 KSceneComponent::GetScreenPosition() const
{
    throw std::logic_error("not implemented");
}

void KSceneComponent::SetParent(KSceneComponent* parent, const ECoordinateSpace keepTransform)
{
    switch (keepTransform)
    {
    case ECoordinateSpace::Relative:
    {
        parent_ = parent;
        break;
    }
    case ECoordinateSpace::World:
    {
        const auto position = GetWorldPosition();
        const auto rotation = GetWorldRotation();
        const auto scale = GetWorldScale();
        parent_ = parent;
        SetWorldPosition(position);
        SetWorldRotation(rotation);
        SetWorldScale(scale);
        break;
    }
    default:
        break;
    }
}

void KSceneComponent::SetRelativePosition(const glm::vec3& relativePosition)
{
    transform_.position = relativePosition;
    eventUpdateTransform_.Broadcast();
}

void KSceneComponent::SetRelativeRotation(const glm::quat& relativeRotation)
{
    transform_.rotation = glm::normalize(relativeRotation);
    eventUpdateTransform_.Broadcast();
}

void KSceneComponent::SetRelativeScale(const glm::vec3& relativeScale)
{
    transform_.scale = relativeScale;
    eventUpdateTransform_.Broadcast();
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

const glm::vec3 KSceneComponent::GetDirection(const KSceneComponent* target) const
{
    return target->GetWorldPosition() - GetWorldPosition();
}

const float KSceneComponent::GetDistance(const KSceneComponent* other) const
{
    return glm::distance(GetWorldPosition(), other->GetWorldPosition());
}
