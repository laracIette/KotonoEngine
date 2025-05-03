#pragma once
#include "Object.h"
#include "Rect.h"
#include "Visibility.h"
#include <kotono_framework/Viewport.h>
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
	const int32_t GetLayer() const;

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(RInterfaceObject* parent, const ECoordinateSpace keepRect);
	void SetLayer(const int32_t layer);

private:
	URect _rect;
	EVisibility _visibility;
	KtViewport* _viewport;
	RInterfaceObject* _parent;
	int32_t _layer;

	void AddBoundsToRenderQueue();
};

