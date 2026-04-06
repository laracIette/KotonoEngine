#pragma once
#include "generated/SceneMeshComponent.generated.h"
#include <kotono_graphics/SceneProxy.h>
#include "SceneComponent.h"
#include "Task.h"

class KtShader;
class KtModel;

class KSceneMeshComponent : public KSceneComponent
{
	GENERATED_KSCENEMESHCOMPONENT()

public:
	KSceneMeshComponent(UPtrOwner* ptrOwner);
	~KSceneMeshComponent() override;

protected:
	void Init() override;
	void Update(const float deltaTime) override;

public:
	UAsset<KtShader> GetShader() const;
	UAsset<KtModel> GetModel() const;

	void SetShader(UAsset<KtShader> shader);
	void SetModel(UAsset<KtModel> model);

	void SetVisibility(const EVisibility visibility, const bool propagateToChildren = false) override;
	void SetMobility(const EMobility mobility) override;

	void Spawn() override;

private:
	void CreateModelProxy();

	void MarkModelProxyTransformDirty();
	void MarkModelProxyScissorDirty();

	void RegisterModelProxy() const;
	void UnregisterModelProxy() const;

	// temp
	void Spin();
	void SetMobilityStatic();
	void SetMobilityDynamic();

private:
	SERIALIZE UAsset<KtShader> shader_;
	SERIALIZE UAsset<KtModel> model_;
	UTask spinTask_;
	USceneProxy* modelProxy_;
};

