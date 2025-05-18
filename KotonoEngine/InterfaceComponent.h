#pragma once
#include "Object.h"
#include "Rect.h"
#include "Visibility.h"

class RInterfaceObject;

class KInterfaceComponent : public KObject
{
	BASECLASS(KObject)

public:
	KInterfaceComponent(RInterfaceObject* owner);

	void Construct() override;
	void Init() override;

	RInterfaceObject* GetOwner() const;
	const URect& GetRect() const;
	URect& GetRect();
	const EVisibility GetVisibility() const;
	const int32_t GetLayer() const;

	void SetVisibility(const EVisibility visibility);
	void SetLayer(const int32_t layer);

private:
	RInterfaceObject* const owner_;
	URect rect_;
	EVisibility visibility_;
	int32_t layer_;

	void AddBoundsToRenderQueue();
};

