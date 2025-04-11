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

	KtEvent<RInterfaceCollider*>& GetEventOverlap();

private:
	std::unordered_set<RInterfaceCollider*> _overlaps;
	KtEvent<RInterfaceCollider*> _eventOverlap;

	KtEvent<> _eventClicked; // todo: maybe

	void UpdateOverlaps();
	void BroadcastOverlaps();
};

