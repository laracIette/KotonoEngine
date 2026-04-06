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
	KInterfaceColliderComponent();
	~KInterfaceColliderComponent() override;

protected:
	void Init() override;

public:
	UEvent<>& GetEventPressed();
	UEvent<>& GetEventReleased();
	UEvent<>& GetEventDown();

	UEvent<UPtr<KInterfaceColliderComponent>>& GetEventOverlap();

private:
	void OnEventMouseLeftButtonPressed();
	void OnEventMouseLeftButtonReleased();
	void OnEventMouseLeftButtonDown();

private:
	KtPool<UPtr<KInterfaceColliderComponent>> overlaps_;
	UEvent<UPtr<KInterfaceColliderComponent>> eventOverlap_;

	UEvent<> eventPressed_;
	UEvent<> eventReleased_;
	UEvent<> eventDown_;

	bool isPressed_;

	size physicsIndex_;
};

