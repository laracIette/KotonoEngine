#pragma once
#include "Object.h"
#include <kotono_framework/Pool.h>

class KtViewport;
class TSceneObject;

class KScene : public KObject
{
	BASECLASS(KObject)

public:
	void Init() override;

	void Load();
	void Unload();
	void Reload();

	void Add(TSceneObject* sceneObject);
	void Remove(TSceneObject* sceneObject);

	void SerializeTo(nlohmann::json& json) const override;
	void DeserializeFrom(const nlohmann::json& json) override;

private:
	KtViewport* viewport_;
	KtPool<TSceneObject*> _sceneObjects;

	TSceneObject* GetSceneObject(const std::string_view type);
};

