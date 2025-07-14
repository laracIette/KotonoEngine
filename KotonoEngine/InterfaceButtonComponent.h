#pragma once
#include "InterfaceComponent.h"
class KInterfaceColliderComponent;
class KInterfaceButtonComponent final : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

public:
	KInterfaceButtonComponent(RInterfaceObject* owner);

	void Init() override;
	void Cleanup() override;

	KtEvent<>& GetEventClicked();

private:
	KInterfaceColliderComponent* collider_;
	KtEvent<> eventClicked_;

	void OnColliderReleased();
};

