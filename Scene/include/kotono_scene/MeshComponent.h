#pragma once
#include "generated/MeshComponent.generated.h"
#include <kotono_core/SceneComponent.h>
#include <kotono_timing/Task.h>
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

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const override;

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

