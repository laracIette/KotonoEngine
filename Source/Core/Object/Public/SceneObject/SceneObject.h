#pragma once
#include "Object/Object.h"

#include "CoordinateSpace.h"
#include <Containers/Set.h>

#include "SceneObject.generated.h"

enum class ESceneVisibility : u32;
struct USceneRenderGraph;
class UScene;
class KSceneComponent; 

class TSceneObject : public KObject
{
	GENERATED()

private:
	friend UScene;

public:
	TSceneObject();
	~TSceneObject() override;

protected:
	virtual void Init();
	virtual void Update(f32 deltaTime);

public:
	auto GetScene() const -> UScene*;
	auto GetRootComponent() const -> UPtr<KSceneComponent>;

	void SetParent(UPtr<TSceneObject> const& parent, ECoordinateSpace keepTransform);

	void AddComponent(UPtr<KSceneComponent> const& component);
	void RemoveComponent(UPtr<KSceneComponent> const& component);

	void Deserialize() override;

	virtual void Spawn();
	virtual void Despawn();

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph, ESceneVisibility visibility) const;

private:
	void InitSceneComponents();
	void UpdateSceneComponents(f32 deltaTime);

private:
	b8 isInit_;
	UScene* scene_;
	WritableProperty(b8, canUpdate_, CanUpdate);
	ReadonlyProperty(UPtr<TSceneObject>, parent_, Parent);
	SERIALIZE USet<UPtr<KSceneComponent>> sceneComponents_;
	SERIALIZE USet<UPtr<TSceneObject>> children_;

	SERIALIZE f32 dummy_;
};