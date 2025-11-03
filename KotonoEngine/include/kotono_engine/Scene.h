#pragma once
#include "Object.h"
#include <kotono_framework/Pool.h>

class TSceneObject;

class KScene : public KObject
{
	BASECLASS(KObject)

protected:
	void Init() override;

public:
	void Load();
	void Unload();
	void Reload();

	void Add(const UPtr<TSceneObject>& sceneObject);
	void Remove(const UPtr<TSceneObject>& sceneObject);

	void SerializeTo(nlohmann::json& json) const override;
	void DeserializeFrom(const nlohmann::json& json) override;

private:
	KtPool<UPtr<TSceneObject>> sceneObjects_;

	UPtr<TSceneObject> GetSceneObject(const std::string_view type);
};

