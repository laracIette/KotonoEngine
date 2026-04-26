#include "InterfaceButtonComponent.h"
#include "InterfaceColliderComponent.h"
#include "InterfaceObject.h"
#include <kotono_object/ObjectManager.h>

KInterfaceButtonComponent::KInterfaceButtonComponent() 
{
	collider_ = UCreate<KInterfaceColliderComponent>{}();
	collider_->SetOwner(GetOwner());
}

void KInterfaceButtonComponent::Init()
{
	Base::Init();

	collider_->GetEventReleased().AddListener(this, &KInterfaceButtonComponent::OnColliderReleased);
	RegisterDelegate(collider_, collider_->GetEventReleased(), this, &KInterfaceButtonComponent::OnColliderReleased);
}

UEvent<>& KInterfaceButtonComponent::GetEventClicked()
{
	return eventClicked_;
}

void KInterfaceButtonComponent::OnColliderReleased()
{
	eventClicked_.Broadcast();
}

#include "generated/InterfaceButtonComponent.generated.inl"
