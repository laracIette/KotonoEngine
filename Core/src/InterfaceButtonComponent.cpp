#include "InterfaceButtonComponent.h"
#include "InterfaceColliderComponent.h"
#include "InterfaceObject.h"
#include "ObjectManager.h"

KInterfaceButtonComponent::KInterfaceButtonComponent(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	collider_ = ObjectManager.Create<KInterfaceColliderComponent>();
	collider_->SetOwner(GetOwner());
}

void KInterfaceButtonComponent::Cleanup()
{
	Base::Cleanup();

	collider_->GetEventReleased().RemoveListener(this, &KInterfaceButtonComponent::OnColliderReleased);
}

void KInterfaceButtonComponent::Init()
{
	Base::Init();

	collider_->GetEventReleased().AddListener(this, &KInterfaceButtonComponent::OnColliderReleased);
}

UEvent<>& KInterfaceButtonComponent::GetEventClicked()
{
	return eventClicked_;
}

void KInterfaceButtonComponent::OnColliderReleased()
{
	eventClicked_.Broadcast();
}
