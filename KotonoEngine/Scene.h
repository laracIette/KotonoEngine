#pragma once
#include "Object.h"
#include <unordered_set>
#include "SceneObject.h"
class OScene : public OObject
{
	BASECLASS(OObject)

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
	KtViewport* _viewport;
	std::unordered_set<TSceneObject*> _sceneObjects;

	TSceneObject* GetSceneObject(const std::string_view type);
};

