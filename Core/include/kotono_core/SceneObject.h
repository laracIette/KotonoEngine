#pragma once
#include "generated/SceneObject.generated.h"
#include <kotono_object/Object.h>
#include "CoordinateSpace.h"
#include <algorithm>
#include <kotono_common/Pool.h>

class URenderContext;
class KScene; 
class KSceneComponent; 

class TSceneObject : public KObject
{
	GENERATED_TSCENEOBJECT()

private:
	friend class KScene;

public:
	TSceneObject();
	~TSceneObject() override;

protected:
	virtual void Init();
	virtual void Update(const float deltaTime);

public:
	UPtr<TSceneObject>& GetParent();
	UPtr<KSceneComponent> GetRootComponent();

	void SetParent(const UPtr<TSceneObject>& parent, const ECoordinateSpace keepTransform);

	template <std::derived_from<KSceneComponent> T>
	UPtr<T> GetComponent() const
	{
		return std::find_first_of(sceneComponents_.begin(), sceneComponents_.end(),
			[](const UPtr<KSceneComponent>& component) { return TryCast<T>(component); }
		);
	}

	void AddComponent(const UPtr<KSceneComponent>& component);
	void RemoveComponent(const UPtr<KSceneComponent>& component);

	void Deserialize() override;

	virtual void Spawn();

	URenderContext& GetRenderContext() const;

private:
	void InitSceneComponents();
	void UpdateSceneComponents(const float deltaTime);

private:

	bool isInit_;
	ReadonlyProperty(UPtr<KScene>, scene_, Scene);
	WritableProperty(bool, canUpdate_, CanUpdate);
	UPtr<TSceneObject> parent_;
	SERIALIZE UPool<UPtr<KSceneComponent>> sceneComponents_;
	SERIALIZE UPool<UPtr<TSceneObject>> children_;
	SERIALIZE size childrenIndex_;
};