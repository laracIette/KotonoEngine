#pragma once
#include "SceneComponent.h"

class KtShader;
class KtModel;
class KTask;
struct KtRenderable3DProxy;

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
	KtModel* model_;
	KTask* spinTask_;
	bool isProxyDirty_;

	void AddModelToRenderQueue();
	void AddWireframeToRenderQueue();

	const KtRenderable3DProxy CreateProxy() const;

	// temp
	void Spin();
};

