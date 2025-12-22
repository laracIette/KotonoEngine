#pragma once
#include "generated/InterfaceButtonComponent.generated.h"
#include "InterfaceComponent.h"
class KInterfaceColliderComponent;
class KInterfaceButtonComponent : public KInterfaceComponent
{
	GENERATED_KINTERFACEBUTTONCOMPONENT()

public:
	KInterfaceButtonComponent(UPtrOwnerBase* ptrOwner);

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

