#pragma once
#include "InterfaceComponent.h"
#include <kotono_framework/Event.h>
#include <unordered_set>
class KInterfaceColliderComponent final : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

public:
	void Init() override;
	void Update() override;
	void Cleanup() override;

	KtEvent<>& GetEventPressed();
	KtEvent<>& GetEventReleased();
	KtEvent<>& GetEventDown();

	KtEvent<KInterfaceColliderComponent*>& GetEventOverlap();

	const bool GetIsMouseOverlapping() const;

private:
	std::unordered_set<KInterfaceColliderComponent*> _overlaps;
	KtEvent<KInterfaceColliderComponent*> _eventOverlap;

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

