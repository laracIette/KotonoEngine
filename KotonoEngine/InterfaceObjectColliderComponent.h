#pragma once
#include "InterfaceObjectComponent.h"
#include <kotono_framework/Event.h>
#include <unordered_set>
class KInterfaceObjectColliderComponent : public KInterfaceObjectComponent
{
	BASECLASS(KInterfaceObjectComponent)

public:
	void Init() override;
	void Update() override;
	void Cleanup() override;

	KtEvent<>& GetEventPressed();
	KtEvent<>& GetEventReleased();
	KtEvent<>& GetEventDown();

	KtEvent<KInterfaceObjectColliderComponent*>& GetEventOverlap();

	const bool GetIsMouseOverlapping() const;

private:
	std::unordered_set<KInterfaceObjectColliderComponent*> _overlaps;
	KtEvent<KInterfaceObjectColliderComponent*> _eventOverlap;

	KtEvent<> _eventPressed;
	KtEvent<> _eventReleased;
	KtEvent<> _eventDown;

	bool _isPressed;

	void UpdateOverlaps();
	void BroadcastOverlaps();

	void OnEventMouseLeftButtonPressed();
	void OnEventMouseLeftButtonReleased();
	void OnEventMouseLeftButtonDown();
};

