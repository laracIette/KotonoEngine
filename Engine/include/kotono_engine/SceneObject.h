#pragma once
#include "generated/SceneObject.generated.h"
#include "Object.h"
#include "CoordinateSpace.h"
#include <kotono_framework/Collection.h>
#include <kotono_common/Pool.h>

class KtWindowViewport;
class KSceneComponent; 

template <class T>
concept SceneComponent = std::is_base_of_v<KSceneComponent, T>;

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
	const UPtr<TSceneObject>& GetParent() const;
	const UPtr<KSceneComponent>& RootComponent() const;

	void SetCanUpdate(const bool canUpdate);
	void SetViewport(KtWindowViewport* viewport);
	void SetParent(const UPtr<TSceneObject>& parent, const ECoordinateSpace keepTransform);

	template <SceneComponent T>
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

	void AddComponent(const UPtr<KSceneComponent>& component);
	void RemoveComponent(const UPtr<KSceneComponent>& component);

private:
	void UpdateSceneComponents();

private:
	bool canUpdate_;
	KtWindowViewport* viewport_;
	UPtr<TSceneObject> parent_;
	UPtr<KSceneComponent> rootComponent_;
	KtPool<UPtr<TSceneObject>> children_;
	KtPool<UPtr<KSceneComponent>> sceneComponents_;
	size_t childrenIndex_;
};