#pragma once
#include "Object.h"
#include "Transform.h"
#include "Visibility.h"
#include <kotono_framework/Viewport.h>
class TSceneObject : public OObject
{
	BASECLASS(OObject)

public:
	void Init() override;

	const UTransform& GetTransform() const;
	UTransform& GetTransform();
	const EVisibility GetVisibility() const;
	KtViewport* GetViewport() const;

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);

private:
	UTransform _transform;
	EVisibility _visibility;
	KtViewport* _viewport;
};