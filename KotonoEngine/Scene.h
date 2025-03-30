#pragma once
#include "Object.h"
#include <vector>
#include "SceneObject.h"
class OScene : public OObject
{
	BASECLASS(OObject)

public:
	void Init() override;

	void Load();
	void Unload();

protected:
	void Serialize(nlohmann::json& json) const override;
	void Deserialize(const nlohmann::json& json) override;

private:
	KtViewport* _viewport;
	std::vector<TSceneObject*> _sceneObjects;
};

