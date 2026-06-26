#pragma once
#include "generated/SceneMeshComponent.generated.h"
#include "SceneComponent.h"
#include "Task.h"

class KtShader;
class KtModel;
class UMaterial;
class USceneProxy;
struct UDrawCall;

class KSceneMeshComponent : public KSceneComponent
{
	GENERATED_KSCENEMESHCOMPONENT()

public:
	KSceneMeshComponent();
	~KSceneMeshComponent() override;

protected:
	void Init() override;
	void Update(const float deltaTime) override;

public:
	const UAsset<KtShader>& GetShader() const;
	const UAsset<KtModel>& GetModel() const;
	const UAsset<UMaterial>& GetMaterial() const;

	void SetShader(const UAsset<KtShader>& shader);
	void SetModel(const UAsset<KtModel>& model);
	void SetMaterial(const UAsset<UMaterial>& material);

	void SetVisibility(const EVisibility visibility, const bool propagateToChildren = false) override;
	void SetMobility(const EMobility mobility) override;

	void Spawn() override;

private:
	void CreateModelProxy();
	void CreateDrawCall();

	void MarkModelProxyTransformDirty();
	void MarkModelProxyScissorDirty();

	void RegisterModelProxy() const;
	void RegisterDrawCall();
	void UnregisterModelProxy() const;
	void UnregisterDrawCall() const;

	// temp
	void Spin();
	void SetMobilityStatic();
	void SetMobilityDynamic();

private:
	SERIALIZE UAsset<KtShader> shader_;
	SERIALIZE UAsset<KtModel> model_;
	SERIALIZE UAsset<UMaterial> material_;
	UTask spinTask_;

	USceneProxy* modelProxy_;
	UDrawCall* drawCall_;
	u32 drawDataIndex_;
	u32 transformIndex_;
	u32 parametersIndex_;
};

