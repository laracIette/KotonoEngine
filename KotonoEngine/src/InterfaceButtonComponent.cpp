#include "InterfaceButtonComponent.h"
#include "InterfaceColliderComponent.h"
#include "InterfaceObject.h"
#include "Engine.h"
#include "ObjectManager.h"

KInterfaceButtonComponent::KInterfaceButtonComponent(UPtrOwnerBase* ptrOwner, const UPtr<RInterfaceObject>& owner) :
	Base(ptrOwner, owner)
{
	collider_ = Engine.ObjectManager().Create<KInterfaceColliderComponent>(Owner());
	Owner()->AddComponent(collider_);
}

void KInterfaceButtonComponent::Init()
{
	Base::Init();

	collider_->GetEventReleased().AddListener(KtDelegate(this, &KInterfaceButtonComponent::OnColliderReleased));
}

void KInterfaceButtonComponent::Cleanup()
{
	Base::Cleanup();

	collider_->GetEventReleased().RemoveListener(KtDelegate(this, &KInterfaceButtonComponent::OnColliderReleased));
}

KtEvent<>& KInterfaceButtonComponent::GetEventClicked()
{
	return eventClicked_;
}

void KInterfaceButtonComponent::OnColliderReleased()
{
	eventClicked_.Broadcast();
}
