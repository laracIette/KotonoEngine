#pragma once
#include "InterfaceComponent.h"
class KInterfaceColliderComponent;
class KInterfaceButtonComponent final : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

public:
	void Construct() override;
	void Init() override;

	KtEvent<>& GetEventClicked();

private:
	KInterfaceColliderComponent* collider_;
	KtEvent<> eventClicked_;

	void OnColliderReleased();
};

