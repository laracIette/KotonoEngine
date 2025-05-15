#pragma once
#include "Object.h"
#include "Rect.h"
#include "Visibility.h"

class RInterfaceObject;

class KInterfaceObjectComponent : public KObject
{
	BASECLASS(KObject)

public:
	void Init() override;

	RInterfaceObject* GetOwner() const;
	const URect& GetRect() const;
	URect& GetRect();
	const EVisibility GetVisibility() const;
	const int32_t GetLayer() const;

	void SetOwner(RInterfaceObject* owner);
	void SetVisibility(const EVisibility visibility);
	void SetLayer(const int32_t layer);

private:
	RInterfaceObject* owner_;
	URect rect_;
	EVisibility visibility_;
	int32_t layer_;
};

