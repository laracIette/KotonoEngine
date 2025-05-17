#pragma once
#include "Object.h"
#include "Transform.h"
#include "Visibility.h"

class TSceneObject;

class KSceneObjectComponent : public KObject
{
	BASECLASS(KObject)

public:
	void Init() override;

	TSceneObject* GetOwner() const;
	UTransform& GetTransform();
	const UTransform& GetTransform() const;
	const EVisibility GetVisibility() const;

	void SetOwner(TSceneObject* owner);
	void GetVisibility(const EVisibility visibility);

private:
	TSceneObject* owner_;
	UTransform transform_;
	EVisibility visibility_;
};

