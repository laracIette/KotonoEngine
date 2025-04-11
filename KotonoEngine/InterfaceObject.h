#pragma once
#include "Object.h"
#include "Rect.h"
#include "Visibility.h"
#include <kotono_framework/Viewport.h>
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

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(RInterfaceObject* parent, const ECoordinateSpace keepRect);

private:
	URect _rect;
	EVisibility _visibility;
	KtViewport* _viewport;
	RInterfaceObject* _parent;

	int _zIndex;//TODO
};

