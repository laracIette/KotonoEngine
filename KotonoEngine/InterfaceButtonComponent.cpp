#include "InterfaceButtonComponent.h"
#include "InterfaceColliderComponent.h"
#include "InterfaceObject.h"

void KInterfaceButtonComponent::Construct()
{
	Base::Construct();

	collider_ = GetOwner()->AddComponent<KInterfaceColliderComponent>();
}

void KInterfaceButtonComponent::Init()
{
	Base::Init();

	const KtDelegate<> colliderReleasedDelegate(this, &KInterfaceButtonComponent::OnColliderReleased);
	ListenEvent(collider_->GetEventReleased(), colliderReleasedDelegate);
}

KtEvent<>& KInterfaceButtonComponent::GetEventClicked()
{
	return eventClicked_;
}

void KInterfaceButtonComponent::OnColliderReleased()
{
	eventClicked_.Broadcast();
}
