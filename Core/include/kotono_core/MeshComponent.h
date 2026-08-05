#pragma once
#include "generated/MeshComponent.generated.h"
#include "SceneComponent.h"
#include "Task.h"
#include <kotono_graphics/DrawCallBuilder.h>
class UShader;
class UModel;
class UMaterial;
struct UDrawCall;

class KMeshComponent : public KSceneComponent
{
	GENERATED_KMESHCOMPONENT()

public:
	KMeshComponent();
	~KMeshComponent() override;

protected:
	void Init() override;
	void Update(const float deltaTime) override;

public:
	const UAsset<UShader>& GetShader() const;
	const UAsset<UModel>& GetModel() const;
	const UAsset<UMaterial>& GetMaterial() const;

	void SetShader(const UAsset<UShader>& shader);
	void SetModel(const UAsset<UModel>& model);
	void SetMaterial(const UAsset<UMaterial>& material);

	void Spawn() override;

private:
	void RegisterDrawCall();
	void UnregisterDrawCall();

	void RefreshDrawCall() const;

	void RefreshDrawCallScissor() const;
	void RefreshDrawCallShaderData() const;
	void RefreshDrawCallModelData() const;
	void RefreshDrawCallMaterialData() const;
	void RefreshDrawCallTransformData() const;

	void OnViewportExtentUpdated(const glm::uvec2 extent) const;

	// temp
	void Spin();
	void SetMobilityStatic();
	void SetMobilityDynamic();

private:
	SERIALIZE UAsset<UShader> shader_;
	SERIALIZE UAsset<UModel> model_;
	SERIALIZE UAsset<UMaterial> material_;
	UTask spinTask_;

	UDrawCallBuilder drawCallBuilder_;
};

