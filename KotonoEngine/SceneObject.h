#pragma once
#include "Object.h"
#include "Transform.h"
#include "Visibility.h"

class KtViewport;

class TSceneObject : public KObject
{
	BASECLASS(KObject)

public:
	void Init() override;

	const UTransform& GetTransform() const;
	UTransform& GetTransform();
	const EVisibility GetVisibility() const;
	KtViewport* GetViewport() const;
	TSceneObject* GetParent() const;

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(TSceneObject* parent, const ECoordinateSpace keepTransform);

	void SerializeTo(nlohmann::json& json) const override;
	void DeserializeFrom(const nlohmann::json& json) override;

private:
	UTransform transform_;
	EVisibility visibility_;
	KtViewport* viewport_;
	TSceneObject* parent_;
};