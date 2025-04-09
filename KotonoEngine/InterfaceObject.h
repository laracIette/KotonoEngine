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

	const URect& GetRect() const;
	URect& GetRect();
	const EVisibility GetVisibility() const;
	KtViewport* GetViewport() const;
	RInterfaceObject* GetParent() const;

	KtEvent& GetEventOverlap();

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(RInterfaceObject* parent, const ECoordinateSpace keepRect);

private:
	URect _rect;
	EVisibility _visibility;
	KtViewport* _viewport;
	RInterfaceObject* _parent;

	std::unordered_set<RInterfaceObject*> _overlaps;
	KtEvent _eventOverlap;

	void UpdateOverlaps();
};

