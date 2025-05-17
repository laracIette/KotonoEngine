#include "SceneObjectComponent.h"
#include "SceneObject.h"

void KSceneObjectComponent::Init()
{
    transform_.SetParent(&owner_->GetTransform(), ECoordinateSpace::Relative);
}

TSceneObject* KSceneObjectComponent::GetOwner() const
{
    return owner_;
}

UTransform& KSceneObjectComponent::GetTransform()
{
    return transform_;
}

const UTransform& KSceneObjectComponent::GetTransform() const
{
    return transform_;
}

const EVisibility KSceneObjectComponent::GetVisibility() const
{
    return visibility_;
}

void KSceneObjectComponent::SetOwner(TSceneObject* owner)
{
    owner_ = owner;
}

void KSceneObjectComponent::GetVisibility(const EVisibility visibility)
{
    visibility_ = visibility;
}
