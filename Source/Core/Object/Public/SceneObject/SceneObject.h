#pragma once
#include "Object/Object.h"

#include "CoordinateSpace.h"
#include <Containers/Set.h>
#include <glm/fwd.hpp>

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
	auto GetRootComponent() const -> UObjectPtr<KSceneComponent>;

	void SetParent(UObjectPtr<TSceneObject> const& parent, ECoordinateSpace keepTransform);

	void AddComponent(UObjectPtr<KSceneComponent> const& component);
	void RemoveComponent(UObjectPtr<KSceneComponent> const& component);

	void Deserialize() override;

	virtual void Spawn();
	virtual void Despawn();

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph, ESceneVisibility visibility) const;
	
	auto GetRayIntersectSceneComponent(glm::vec3 const& start, glm::vec3 const& direction) const -> UObjectPtr<KSceneComponent>;

private:
	void InitSceneComponents();
	void UpdateSceneComponents(f32 deltaTime);

private:
	b8 isInit_;
	UScene* scene_;
	WritableProperty(b8, canUpdate_, CanUpdate);
	ReadonlyProperty(UObjectPtr<TSceneObject>, parent_, Parent);
	SERIALIZE USet<UObjectPtr<KSceneComponent>> sceneComponents_;
	SERIALIZE USet<UObjectPtr<TSceneObject>> children_;

	SERIALIZE f32 dummy_;
};