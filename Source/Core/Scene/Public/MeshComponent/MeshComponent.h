#pragma once
#include "MeshComponent.generated.h"
#include <SceneComponent/SceneComponent.h>

#include <Task/Task.h>

class KMeshComponent : public KSceneComponent
{
	GENERATED_KMESHCOMPONENT()

public:
	KMeshComponent();
	~KMeshComponent() override;

protected:
	void Init() override;
	void Update(f32 deltaTime) override;

public:
	void Spawn() override;
	void Despawn() override;

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph, ESceneVisibility visibility) const override;

private:
	// temp
	void Spin(f32 deltaTime);
	void SetMobilityStatic();
	void SetMobilityDynamic();

private:
	SERIALIZE WritableProperty(UPath, shader_, Shader);
	SERIALIZE WritableProperty(UPath, model_, Model);
	SERIALIZE WritableProperty(UPath, material_, Material);
	UTask spinTask_;
};

