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

	collider_->GetEventReleased().RemoveListener(KtDelegate(this, &KInterfaceButtonComponent::OnColliderReleased));
}

void KInterfaceButtonComponent::Init()
{
	Base::Init();

	collider_->GetEventReleased().AddListener(KtDelegate(this, &KInterfaceButtonComponent::OnColliderReleased));
}

KtEvent<>& KInterfaceButtonComponent::GetEventClicked()
{
	return eventClicked_;
}

void KInterfaceButtonComponent::OnColliderReleased()
{
	eventClicked_.Broadcast();
}
