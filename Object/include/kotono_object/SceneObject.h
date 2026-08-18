#pragma once
#include "generated/SceneObject.generated.h"
#include "Object.h"
#include "CoordinateSpace.h"
#include <algorithm>
#include <kotono_common/Set.h>
struct USceneRenderGraph;
class UScene;
class KSceneComponent; 
class TSceneObject : public KObject
{
	GENERATED_TSCENEOBJECT()

private:
	friend UScene;

public:
	TSceneObject();
	~TSceneObject() override;

protected:
	virtual void Init();
	virtual void Update(f32 deltaTime);

public:
	UScene* GetScene() const;
	UPtr<TSceneObject> const& GetParent() const;
	UPtr<KSceneComponent> GetRootComponent() const;

	void SetParent(UPtr<TSceneObject> const& parent, ECoordinateSpace keepTransform);

	template <std::derived_from<KSceneComponent> T>
	UPtr<T> GetComponent() const
	{
		return std::find_first_of(sceneComponents_.begin(), sceneComponents_.end(),
			[](const UPtr<KSceneComponent>& component) { return TryCast<T>(component); }
		);
	}

	void AddComponent(UPtr<KSceneComponent> const& component);
	void RemoveComponent(UPtr<KSceneComponent> const& component);

	void Deserialize() override;

	virtual void Spawn();

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const;

private:
	void InitSceneComponents();
	void UpdateSceneComponents(f32 deltaTime);

private:
	b8 isInit_;
	UScene* scene_;
	WritableProperty(b8, canUpdate_, CanUpdate);
	UPtr<TSceneObject> parent_;
	SERIALIZE USet<UPtr<KSceneComponent>> sceneComponents_;
	SERIALIZE USet<UPtr<TSceneObject>> children_;
};