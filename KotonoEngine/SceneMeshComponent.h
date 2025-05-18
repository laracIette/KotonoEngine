#pragma once
#include "SceneComponent.h"

class KtShader;
class KtModel;
class KTask;

class KSceneMeshComponent : public KSceneComponent
{
	BASECLASS(KSceneComponent)

public:
	void Construct() override;
	void Init() override;
	void Update() override;
	void Cleanup() override;

	KtShader* GetShader() const;
	KtModel* GetModel() const;

	void SetShader(KtShader* shader);
	void SetModel(KtModel* model);

	void SerializeTo(nlohmann::json& json) const override;
	void DeserializeFrom(const nlohmann::json& json) override;


private:
	KtShader* shader_;
	KtModel* _model;
	KTask* _spinTask;

	void AddModelToRenderQueue();
	void AddWireframeToRenderQueue();

	// temp
	void Spin();
};

