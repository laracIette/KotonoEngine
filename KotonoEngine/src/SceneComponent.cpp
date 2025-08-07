#include "SceneComponent.h"
#include "SceneObject.h"
#include "log.h"
#include <stdexcept>

KSceneComponent::KSceneComponent(TSceneObject* owner) : 
    Base(),
    owner_(owner)
{
    if (GetOwner()->GetRootComponent() != this)
    {
        SetParent(GetOwner()->GetRootComponent(), ECoordinateSpace::Relative);
    }
}

void KSceneComponent::Init()
{
    Base::Init();
}

void KSceneComponent::Cleanup()
{
    Base::Cleanup();
    
    GetOwner()->RemoveComponent(this);
}

TSceneObject* KSceneComponent::GetOwner() const
{
    return owner_;
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

bool KSceneComponent::GetCanSetTransform() const
{
    return mobility_ == EMobility::Dynamic || !GetIsConstructed();
}

KtEvent<>& KSceneComponent::GetEventUpdateTransform()
{
    return eventUpdateTransform_;
}

void KSceneComponent::SetVisibility(const EVisibility visibility)
{
    visibility_ = visibility;
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
        return parent_->GetWorldPosition() + GetRelativePosition();
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

glm::vec3 KSceneComponent::GetRightVector() const
{
    return GetWorldRotation() * glm::vec3(-1.0f, 0.0f, 0.0f);
}

glm::vec3 KSceneComponent::GetForwardVector() const
{
    return GetWorldRotation() * glm::vec3(0.0f, 0.0f, 1.0f);
}

glm::vec3 KSceneComponent::GetUpVector() const
{
    return GetWorldRotation() * glm::vec3(0.0f, -1.0f, 0.0f);
}

glm::mat4 KSceneComponent::GetTranslationMatrix() const
{
    return glm::translate(glm::identity<glm::mat4>(), GetWorldPosition());
}

glm::mat4 KSceneComponent::GetRotationMatrix() const
{
    return glm::mat4_cast(GetWorldRotation());
}

glm::mat4 KSceneComponent::GetScaleMatrix() const
{
    return glm::scale(glm::identity<glm::mat4>(), GetWorldScale());
}

glm::mat4 KSceneComponent::GetModelMatrix() const
{
    return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
}

glm::vec3 KSceneComponent::GetScreenPosition() const
{
    throw std::logic_error("not implemented");
}

void KSceneComponent::SetParent(KSceneComponent* parent, const ECoordinateSpace keepTransform)
{
    if (!GetCanSetTransform())
    {
        KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_HIGH, "can't set parent for %s, its mobility is static", GetName().c_str());
        return;
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
        const auto position = GetWorldPosition();
        const auto rotation = GetWorldRotation();
        const auto scale = GetWorldScale();
        parent_ = parent;
        SetWorldPosition(position);
        SetWorldRotation(rotation);
        SetWorldScale(scale);
        break;
    }
    }
}

void KSceneComponent::SetRelativePosition(const glm::vec3& relativePosition)
{
    if (!GetCanSetTransform())
    {
        KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_HIGH, "can't set position for %s, its mobility is static", GetName().c_str());
        return;
    }

    if (transform_.position == relativePosition)
    {
        return;
    }

    transform_.position = relativePosition;
    eventUpdateTransform_.Broadcast();
}

void KSceneComponent::SetRelativeRotation(const glm::quat& relativeRotation)
{
    if (!GetCanSetTransform())
    {
        KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_HIGH, "can't set rotation for %s, its mobility is static", GetName().c_str());
        return;
    }

    if (transform_.rotation == relativeRotation)
    {
        return;
    }

    transform_.rotation = glm::normalize(relativeRotation);
    eventUpdateTransform_.Broadcast();
}

void KSceneComponent::SetRelativeScale(const glm::vec3& relativeScale)
{
    if (!GetCanSetTransform())
    {
        KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_HIGH, "can't set scale for %s, its mobility is static", GetName().c_str());
        return;
    }

    if (transform_.scale == relativeScale)
    {
        return;
    }

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

glm::vec3 KSceneComponent::GetDirection(const KSceneComponent* target) const
{
    return target->GetWorldPosition() - GetWorldPosition();
}

float KSceneComponent::GetDistance(const KSceneComponent* other) const
{
    return glm::distance(GetWorldPosition(), other->GetWorldPosition());
}
