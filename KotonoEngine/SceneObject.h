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
	TSceneObject* GetParent() const;

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(TSceneObject* parent, const ETransformSpace keepTransform);

	void SerializeTo(nlohmann::json& json) const override;
	void DeserializeFrom(const nlohmann::json& json) override;

private:
	UTransform _transform;
	EVisibility _visibility;
	KtViewport* _viewport;
	TSceneObject* _parent;
};