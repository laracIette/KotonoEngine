#pragma once
#include "generated/SceneObject.generated.h"
#include <kotono_object/Object.h>
#include "CoordinateSpace.h"
#include <algorithm>
#include <kotono_common/Pool.h>

class GWindowViewport;
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
	bool GetCanUpdate() const;
	GWindowViewport* GetViewport() const;
	UPtr<TSceneObject>& GetParent();
	UPtr<KSceneComponent> GetRootComponent();

	void SetCanUpdate(const bool canUpdate);
	void SetViewport(GWindowViewport* viewport);
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

private:
	void InitSceneComponents();
	void UpdateSceneComponents(const float deltaTime);

private:
	bool isInit_;
	bool canUpdate_;
	GWindowViewport* viewport_;
	UPtr<TSceneObject> parent_;
	SERIALIZE UPool<UPtr<KSceneComponent>> sceneComponents_;
	SERIALIZE UPool<UPtr<TSceneObject>> children_;
	SERIALIZE size childrenIndex_;
};