#pragma once
#include "InterfaceComponent.h"
#include <kotono_framework/Event.h>
#include <kotono_framework/Pool.h>
class KInterfaceColliderComponent final : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

	friend class SInterfacePhysicsManager;

public:
	KInterfaceColliderComponent(RInterfaceObject* owner);

protected:
	void Init() override;
	void Update() override;
	void Cleanup() override;

public:
	KtEvent<>& GetEventPressed();
	KtEvent<>& GetEventReleased();
	KtEvent<>& GetEventDown();

	KtEvent<KInterfaceColliderComponent*>& GetEventOverlap();

	bool GetIsMouseOverlapping() const;

private:
	KtPool<KInterfaceColliderComponent*> overlaps_;
	KtEvent<KInterfaceColliderComponent*> eventOverlap_;

	KtEvent<> eventPressed_;
	KtEvent<> eventReleased_;
	KtEvent<> eventDown_;

	bool isPressed_;

	size_t physicsIndex_;

	void OnEventMouseLeftButtonPressed();
	void OnEventMouseLeftButtonReleased();
	void OnEventMouseLeftButtonDown();
};

