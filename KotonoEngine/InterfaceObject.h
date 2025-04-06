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

	const URect& GetRect() const;
	URect& GetRect();
	const EVisibility GetVisibility() const;
	KtViewport* GetViewport() const;
	RInterfaceObject* GetParent() const;

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(RInterfaceObject* parent);

private:
	URect _rect;
	EVisibility _visibility;
	KtViewport* _viewport;
	RInterfaceObject* _parent;
};

