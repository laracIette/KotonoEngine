#pragma once
#include "InterfaceComponent.h"
#include <kotono_framework/Event.h>
#include <kotono_framework/Pool.h>
class KInterfaceColliderComponent final : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

	friend class SInterfacePhysicsManager;

public:
	KInterfaceColliderComponent(UPtrOwnerBase* ptrOwner, const UPtr<RInterfaceObject>& owner);

protected:
	void Init() override;
	void Cleanup() override;

public:
	KtEvent<>& GetEventPressed();
	KtEvent<>& GetEventReleased();
	KtEvent<>& GetEventDown();

	KtEvent<UPtr<KInterfaceColliderComponent>>& GetEventOverlap();

private:
	KtPool<UPtr<KInterfaceColliderComponent>> overlaps_;
	KtEvent<UPtr<KInterfaceColliderComponent>> eventOverlap_;

	KtEvent<> eventPressed_;
	KtEvent<> eventReleased_;
	KtEvent<> eventDown_;

	bool isPressed_;

	size_t physicsIndex_;

	void OnEventMouseLeftButtonPressed();
	void OnEventMouseLeftButtonReleased();
	void OnEventMouseLeftButtonDown();
};

