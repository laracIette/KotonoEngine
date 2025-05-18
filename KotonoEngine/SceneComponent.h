#pragma once
#include "Object.h"
#include "Transform.h"
#include "Visibility.h"

class TSceneObject;

class KSceneComponent : public KObject
{
	BASECLASS(KObject)

public:
	KSceneComponent(TSceneObject* owner);

	void Init() override;

	TSceneObject* GetOwner() const;
	UTransform& GetTransform();
	const UTransform& GetTransform() const;
	const EVisibility GetVisibility() const;

	void GetVisibility(const EVisibility visibility);

private:
	TSceneObject* const owner_;
	UTransform transform_;
	EVisibility visibility_;
};

