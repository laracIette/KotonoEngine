#include "InterfaceButtonComponent.h"
#include "InterfaceColliderComponent.h"
#include "InterfaceObject.h"

KInterfaceButtonComponent::KInterfaceButtonComponent(RInterfaceObject* owner) :
	Base(owner)
{
	collider_ = GetOwner()->AddComponent<KInterfaceColliderComponent>();
}

void KInterfaceButtonComponent::Init()
{
	Base::Init();

	collider_->GetEventReleased().AddListener(KtDelegate<>(this, &KInterfaceButtonComponent::OnColliderReleased));
}

void KInterfaceButtonComponent::Cleanup()
{
	Base::Cleanup();

	collider_->GetEventReleased().RemoveListener(KtDelegate<>(this, &KInterfaceButtonComponent::OnColliderReleased));
}

KtEvent<>& KInterfaceButtonComponent::GetEventClicked()
{
	return eventClicked_;
}

void KInterfaceButtonComponent::OnColliderReleased()
{
	eventClicked_.Broadcast();
}
