#pragma once
#include "generated/SceneMeshComponent.generated.h"
#include <kotono_framework/Renderable3DProxy.h>
#include "SceneComponent.h"

class KtShader;
class KtModel;
class KTask;

class KSceneMeshComponent : public KSceneComponent
{
	GENERATED_KSCENEMESHCOMPONENT()

public:
	KSceneMeshComponent(UPtrOwnerBase* ptrOwner, const UPtr<TSceneObject>& owner);

protected:
	void Init() override;
	void Cleanup() override;

public:
	KtShader* GetShader() const;
	KtModel* GetModel() const;

	void SetShader(KtShader* shader);
	void SetModel(KtModel* model); 

	void SetVisibility(const EVisibility visibility, const bool propagateToChildren = false) override;
	void SetMobility(const EMobility mobility) override;

	//void SerializeTo(nlohmann::json& json) const override;
	//void DeserializeFrom(const nlohmann::json& json) override;

	void Spawn() override;

private:
	KtShader* shader_;
	KtModel* model_;
	UPtr<KTask> spinTask_;
	KtRenderable3DProxy modelProxy_;

	void CreateModelProxy();

	void MarkModelProxyTransformDirty();

	void RegisterModelProxy();
	void UnregisterModelProxy();

	// temp
	void Spin();
	void SetMobilityStatic();
	void SetMobilityDynamic();
};

