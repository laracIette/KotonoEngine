#pragma once
#include "Object.h"
#include "Rect.h"
#include "Visibility.h"
class RInterfaceObject : public OObject
{
	BASECLASS(OObject)

public:
	RInterfaceObject();

	const URect& GetRect() const;
	URect& GetRect();

	const EVisibility GetVisibility() const;
	void SetVisibility(const EVisibility visibility);

protected:
	URect _rect;
	EVisibility _visibility;
};

