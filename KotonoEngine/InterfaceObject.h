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
	KtViewport* GetViewport() const;

	const EVisibility GetVisibility() const;
	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);

private:
	URect _rect;
	EVisibility _visibility;
	KtViewport* _viewport;
};

