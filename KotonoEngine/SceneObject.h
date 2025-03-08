#pragma once
#include "Object.h"
#include "Transform.h"
#include "Visibility.h"
class TSceneObject : public OObject
{
	BASECLASS(OObject)

public:
	TSceneObject();

	const UTransform& GetTransform() const;
	UTransform& GetTransform();

	const EVisibility GetVisibility() const;
	void SetVisibility(const EVisibility visibility);

protected:
	UTransform _transform;
	EVisibility _visibility;
};