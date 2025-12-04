#include "SceneComponent.h"
#include "SceneObject.h"
#include "log.h"
#include <stdexcept>
#include "Engine.h"
#include <nlohmann/json.hpp>

KSceneComponent::KSceneComponent(UPtrOwnerBase* ptrOwner, const UPtr<TSceneObject>& owner) :
    Base(ptrOwner),
    owner_(owner),
    visibility_(EVisibility::Visible),
    modelMatrix_([this]() { return TranslationMatrix() * RotationMatrix() * ScaleMatrix(); })
{
    eventTransformUpdated_.AddListener(KtDelegate(&modelMatrix_, &KtCached<glm::mat4>::MarkDirty));
    
    if (Owner()->RootComponent() != this)
    {
        SetParent(Owner()->RootComponent(), ECoordinateSpace::Relative);
    }
}

void KSceneComponent::Init()
{
    Base::Init();
}

void KSceneComponent::Cleanup()
{
    Owner()->RemoveComponent(Ptr<KSceneComponent>());

    Base::Cleanup();
}

void KSceneComponent::Update()
{
}

const UPtr<TSceneObject>& KSceneComponent::Owner() const
{
    return owner_;
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

bool KSceneComponent::GetCanSetTransform() const
{
    return mobility_ == EMobility::Dynamic || !IsConstructed();
}

KtEvent<>& KSceneComponent::EventTransformUpdated()
{
    return eventTransformUpdated_;
}

const glm::vec3& KSceneComponent::GetSpawnPosition() const
{
    return spawnTransform_.position;
}

const glm::quat& KSceneComponent::GetSpawnRotation() const
{
    return spawnTransform_.rotation;
}

const glm::vec3& KSceneComponent::GetSpawnScale() const
{
    return spawnTransform_.scale;
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
        for (const auto& sceneComponent : children_)
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

glm::vec3 KSceneComponent::RightVector() const
{
    return GetWorldRotation() * glm::vec3(-1.0f, 0.0f, 0.0f);
}

glm::vec3 KSceneComponent::ForwardVector() const
{
    return GetWorldRotation() * glm::vec3(0.0f, 0.0f, 1.0f);
}

glm::vec3 KSceneComponent::UpVector() const
{
    return GetWorldRotation() * glm::vec3(0.0f, -1.0f, 0.0f);
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

glm::mat4 KSceneComponent::ModelMatrix()
{
    return modelMatrix_;
}

glm::vec3 KSceneComponent::GetScreenPosition() const
{
    throw std::logic_error("not implemented");
}

void KSceneComponent::SetParent(const UPtr<KSceneComponent>& parent, const ECoordinateSpace keepTransform)
{
    if (parent_ == parent)
    {
        return;
    }

    if (!GetCanSetTransform())
    {
        KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_HIGH, "can't set parent for %s, its mobility is static", GetName().c_str());
        return;
    }

    if (parent_)
    {
        const size_t index{ childrenIndex_ };
        if (parent_->children_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
        {
            parent_->children_[index]->childrenIndex_ = index;
        }
    }
    if (parent)
    {
        parent->children_.Add(Ptr<KSceneComponent>());
        childrenIndex_ = parent->children_.LastIndex();
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

void KSceneComponent::SetSpawnPosition(const glm::vec3& spawnPosition)
{
    spawnTransform_.position = spawnPosition;
}

void KSceneComponent::SetSpawnRotation(const glm::quat& spawnRotation)
{
    spawnTransform_.rotation = spawnRotation;
}

void KSceneComponent::SetSpawnScale(const glm::vec3& spawnScale)
{
    spawnTransform_.scale = spawnScale;
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
    eventTransformUpdated_.Broadcast();
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
    eventTransformUpdated_.Broadcast();
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

glm::vec3 KSceneComponent::GetDirection(const UPtr<KSceneComponent>& target) const
{
    return target->GetWorldPosition() - GetWorldPosition();
}

float KSceneComponent::GetDistance(const UPtr<KSceneComponent>& other) const
{
    return glm::distance(GetWorldPosition(), other->GetWorldPosition());
}

void KSceneComponent::Spawn()
{
    SetRelativePosition(GetSpawnPosition());
    SetRelativeRotation(GetSpawnRotation());
    SetRelativeScale(GetSpawnScale());

    for (const auto& sceneComponent : children_)
    {
        sceneComponent->Spawn();
    }
}

//void KSceneComponent::SerializeTo(nlohmann::json& json) const
//{
//    Base::SerializeTo(json);
//
//    json["transform_"]["position"]["x"] = transform_.position.x;
//    json["transform_"]["position"]["y"] = transform_.position.y;
//    json["transform_"]["position"]["z"] = transform_.position.z;
//    json["transform_"]["rotation"]["w"] = transform_.rotation.w;
//    json["transform_"]["rotation"]["x"] = transform_.rotation.x;
//    json["transform_"]["rotation"]["y"] = transform_.rotation.y;
//    json["transform_"]["rotation"]["z"] = transform_.rotation.z;
//    json["transform_"]["scale"]["x"] = transform_.scale.x;
//    json["transform_"]["scale"]["y"] = transform_.scale.y;
//    json["transform_"]["scale"]["z"] = transform_.scale.z;
//}

//void KSceneComponent::DeserializeFrom(const nlohmann::json& json)
//{
//    Base::DeserializeFrom(json);
//
//    SetRelativePosition({
//        json["transform_"]["position"]["x"],
//        json["transform_"]["position"]["y"],
//        json["transform_"]["position"]["z"]
//    });
//    SetRelativeRotation({
//        json["transform_"]["rotation"]["w"],
//        json["transform_"]["rotation"]["x"],
//        json["transform_"]["rotation"]["y"],
//        json["transform_"]["rotation"]["z"]
//    });
//    SetRelativeScale({
//        json["transform_"]["scale"]["x"],
//        json["transform_"]["scale"]["y"],
//        json["transform_"]["scale"]["z"]
//    });
//}
