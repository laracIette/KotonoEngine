#pragma once
#include "generated/SceneMeshComponent.generated.h"
#include <kotono_graphics/SceneRenderableProxy.h>
#include "SceneComponent.h"
#include "Task.h"

class KtShader;
class KtModel;

class KSceneMeshComponent : public KSceneComponent
{
	GENERATED_KSCENEMESHCOMPONENT()

public:
	KSceneMeshComponent(UPtrOwnerBase* ptrOwner);

protected:
	void Cleanup() override;

	void Init() override;
	void Update(const float delta) override;

public:
	KtShader* GetShader() const;
	KtModel* GetModel() const;

	void SetShader(KtShader* shader);
	void SetModel(KtModel* model); 

	void SetVisibility(const EVisibility visibility, const bool propagateToChildren = false) override;
	void SetMobility(const EMobility mobility) override;

	void Spawn() override;

private:
	SERIALIZE KtShader* shader_;
	SERIALIZE KtModel* model_;
	UTask spinTask_;
	KtSceneRenderableProxy modelProxy_;

	void CreateModelProxy();

	void MarkModelProxyTransformDirty();

	void RegisterModelProxy();
	void UnregisterModelProxy();

	// temp
	void Spin();
	void SetMobilityStatic();
	void SetMobilityDynamic();
};

