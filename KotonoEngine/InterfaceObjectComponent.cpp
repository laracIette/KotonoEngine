#include "InterfaceObjectComponent.h"
#include "InterfaceObject.h"

void KInterfaceObjectComponent::Init()
{
    Base::Init();

    rect_.SetParent(&owner_->GetRect(), ECoordinateSpace::Relative);
    visibility_ = EVisibility::EditorAndGame;
}

RInterfaceObject* KInterfaceObjectComponent::GetOwner() const
{
    return owner_;
}

const URect& KInterfaceObjectComponent::GetRect() const
{
    return rect_;
}

URect& KInterfaceObjectComponent::GetRect() 
{
    return rect_;
}

const EVisibility KInterfaceObjectComponent::GetVisibility() const
{
    return visibility_;
}

const int32_t KInterfaceObjectComponent::GetLayer() const
{
    return owner_->GetLayer() + layer_ + 1;
}

void KInterfaceObjectComponent::SetOwner(RInterfaceObject* owner)
{
    owner_ = owner;
}

void KInterfaceObjectComponent::SetVisibility(const EVisibility visibility)
{
    visibility_ = visibility;
}

void KInterfaceObjectComponent::SetLayer(const int32_t layer)
{
    layer_ = layer;
}
