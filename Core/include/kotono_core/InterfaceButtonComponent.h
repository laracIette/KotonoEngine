#pragma once
#include "generated/InterfaceButtonComponent.generated.h"
#include "InterfaceComponent.h"
class KInterfaceColliderComponent;
class KInterfaceButtonComponent : public KInterfaceComponent
{
	GENERATED_KINTERFACEBUTTONCOMPONENT()

public:
	KInterfaceButtonComponent(UPtrOwner* ptrOwner);

protected:
	void Init() override;

public:
	UEvent<>& GetEventClicked();

private:
	void OnColliderReleased();

private:
	UPtr<KInterfaceColliderComponent> collider_;
	UEvent<> eventClicked_;
};

