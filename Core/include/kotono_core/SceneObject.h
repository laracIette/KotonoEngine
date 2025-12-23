#pragma once
#include "generated/SceneObject.generated.h"
#include "Object.h"
#include "CoordinateSpace.h"
#include <kotono_common/Collection.h>
#include <kotono_common/Pool.h>

class KtWindowViewport;
class KSceneComponent; 

class TSceneObject : public KObject
{
	GENERATED_TSCENEOBJECT()

private:
	friend class KScene;

public:
	TSceneObject(UPtrOwnerBase* ptrOwner);

protected:
	void Init() override;
	void Cleanup() override;

	virtual void Update();

public:
	bool GetCanUpdate() const;
	KtWindowViewport* GetViewport() const;
	UPtr<TSceneObject>& GetParent();
	UPtr<KSceneComponent>& RootComponent();

	void SetCanUpdate(const bool canUpdate);
	void SetViewport(KtWindowViewport* viewport);
	void SetParent(const UPtr<TSceneObject>& parent, const ECoordinateSpace keepTransform);

	template <std::derived_from<KSceneComponent> T>
	UPtr<T> GetComponent() const
	{
		auto components = KtCollection(sceneComponents_.begin(), sceneComponents_.end());
		components.AddFilter([](const UPtr<KSceneComponent>& component) { return dynamic_cast<T*>(component.Get()); });
		if (components.Empty())
		{
			return nullptr;
		}
		return components.GetFirst();
	}

	void AddComponent(UPtr<KSceneComponent> component);
	void RemoveComponent(const UPtr<KSceneComponent>& component);

	void Serialize() const override;
	void Deserialize() override;

	virtual void Spawn();

private:
	void UpdateSceneComponents();

private:
	bool canUpdate_;
	KtWindowViewport* viewport_;
	UPtr<TSceneObject> parent_;
	SERIALIZE UPtr<KSceneComponent> rootComponent_;
	SERIALIZE KtPool<UPtr<KSceneComponent>> sceneComponents_;
	SERIALIZE KtPool<UPtr<TSceneObject>> children_;
	SERIALIZE size_t childrenIndex_;
};