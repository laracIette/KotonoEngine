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
	void Cleanup() override;

	void Init() override;

public:
	KtEvent<>& GetEventClicked();

private:
	void OnColliderReleased();

private:
	UPtr<KInterfaceColliderComponent> collider_;
	KtEvent<> eventClicked_;
};

