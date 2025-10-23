#pragma once
#include "InterfaceComponent.h"
class KInterfaceColliderComponent;
class KInterfaceButtonComponent final : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

public:
	KInterfaceButtonComponent(UPtrOwnerBase* ptrOwner, const UPtr<RInterfaceObject>& owner);

protected:
	void Init() override;
	void Cleanup() override;

public:
	KtEvent<>& GetEventClicked();

private:
	UPtr<KInterfaceColliderComponent> collider_;
	KtEvent<> eventClicked_;

	void OnColliderReleased();
};

