#pragma once
#include "generated/MeshComponent.generated.h"
#include <kotono_object/SceneComponent.h>
#include "Task.h"
class KMeshComponent : public KSceneComponent
{
	GENERATED_KMESHCOMPONENT()

public:
	KMeshComponent();
	~KMeshComponent() override;

protected:
	void Init() override;
	void Update(const f32 deltaTime) override;

public:
	const UPath& GetShader() const;
	const UPath& GetModel() const;
	const UPath& GetMaterial() const;

	void SetShader(const UPath& shader);
	void SetModel(const UPath& model);
	void SetMaterial(const UPath& material);

	void Spawn() override;

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const override;

private:
	// temp
	void Spin(const f32 deltaTime);
	void SetMobilityStatic();
	void SetMobilityDynamic();

private:
	SERIALIZE UPath shader_;
	SERIALIZE UPath model_;
	SERIALIZE UPath material_;
	UTask spinTask_;
};

