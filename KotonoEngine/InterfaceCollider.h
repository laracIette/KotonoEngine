#pragma once
#include "InterfaceObject.h"
#include <kotono_framework/Event.h>
#include <unordered_set>
class RInterfaceCollider : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	void Init() override;
	void Update() override;
	void Cleanup() override;

	KtEvent<>& GetEventPressed();
	KtEvent<>& GetEventReleased();
	KtEvent<>& GetEventDown();

	KtEvent<RInterfaceCollider*>& GetEventOverlap();

	const bool GetIsMouseOverlapping() const;

private:
	std::unordered_set<RInterfaceCollider*> _overlaps;
	KtEvent<RInterfaceCollider*> _eventOverlap;

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

