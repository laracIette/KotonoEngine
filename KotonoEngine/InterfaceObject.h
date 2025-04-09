#pragma once
#include "Object.h"
#include "Rect.h"
#include "Visibility.h"
#include <kotono_framework/Viewport.h>
#include <kotono_framework/Event.h>
#include <unordered_set>
class RInterfaceObject : public OObject
{
	BASECLASS(OObject)

public:
	void Init() override;
	void Update() override;
	void Cleanup() override;

	const URect& GetRect() const;
	URect& GetRect();
	const EVisibility GetVisibility() const;
	KtViewport* GetViewport() const;
	RInterfaceObject* GetParent() const;

	// interface objects dont necesarily have anything to draw
	// TODO: move colliders / overlap events to classes who directly need them 

	KtEvent<RInterfaceObject*>& GetEventOverlap();

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(RInterfaceObject* parent, const ECoordinateSpace keepRect);

private:
	void OnEventOverlap(RInterfaceObject* other);
	void OnEventMouseLeftButtonDown();

	URect _rect;
	EVisibility _visibility;
	KtViewport* _viewport;
	RInterfaceObject* _parent;

	std::unordered_set<RInterfaceObject*> _overlaps;
	KtEvent<RInterfaceObject*> _eventOverlap;
	KtEvent<> _eventClicked;

	void UpdateOverlaps();
	void BroadcastOverlaps();
};

