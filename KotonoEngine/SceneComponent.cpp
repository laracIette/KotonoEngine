#include "SceneComponent.h"
#include "SceneObject.h"

KSceneComponent::KSceneComponent(TSceneObject* owner)
    : owner_(owner)
{
}

void KSceneComponent::Init()
{
    transform_.SetParent(&owner_->GetTransform(), ECoordinateSpace::Relative);
}

TSceneObject* KSceneComponent::GetOwner() const
{
    return owner_;
}

UTransform& KSceneComponent::GetTransform()
{
    return transform_;
}

const UTransform& KSceneComponent::GetTransform() const
{
    return transform_;
}

const EVisibility KSceneComponent::GetVisibility() const
{
    return visibility_;
}

void KSceneComponent::GetVisibility(const EVisibility visibility)
{
    visibility_ = visibility;
}
