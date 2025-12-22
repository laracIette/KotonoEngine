#pragma once
#include "generated/InterfaceColliderComponent.generated.h"
#include "InterfaceComponent.h"
#include <kotono_common/Event.h>
#include <kotono_common/Pool.h>
class KInterfaceColliderComponent : public KInterfaceComponent
{
	GENERATED_KINTERFACECOLLIDERCOMPONENT()

	friend class SInterfacePhysicsManager;

public:
	KInterfaceColliderComponent(UPtrOwnerBase* ptrOwner);

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

