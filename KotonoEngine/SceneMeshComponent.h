#pragma once
#include <kotono_framework/Renderable3DProxy.h>
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

	void SetMobility(const EMobility mobility) override;

private:
	KtShader* shader_;
	KtModel* model_;
	KTask* spinTask_;
	KtRenderable3DProxy proxy_;

	void InitSpin();

	void InitModelProxy();
	void CreateModelProxy();

	void MarkModelProxyTransformDirty();

	void RegisterProxies();
	void RemoveProxies();

	// temp
	void Spin();
	void SetMobilityStatic();
	void SetMobilityDynamic();
};

