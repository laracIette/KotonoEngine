#pragma once
#include "Object.h"
#include "Visibility.h"
#include "CoordinateSpace.h"
#include <kotono_framework/Collection.h>
#include <kotono_framework/Pool.h>

class KtWindowViewport;
class KSceneComponent; 

template <class T>
concept SceneComponent = std::is_base_of_v<KSceneComponent, T>;

class TSceneObject : public KObject
{
	BASECLASS(KObject)

private:
	friend class KScene;

public:
	TSceneObject(UPtrOwnerBase* ptrOwner);

protected:
	void Init() override;
	void Cleanup() override;

public:
	EVisibility GetVisibility() const;
	KtWindowViewport* GetViewport() const;
	const UPtr<TSceneObject>& GetParent() const;
	const UPtr<KSceneComponent>& RootComponent() const;

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtWindowViewport* viewport);
	void SetParent(const UPtr<TSceneObject>& parent, const ECoordinateSpace keepTransform);

	template <SceneComponent T>
	UPtr<T> GetComponent() const
	{
		auto components = KtCollection(components_.begin(), components_.end());
		components.AddFilter([](const UPtr<KSceneComponent>& component) { return dynamic_cast<T*>(component.Get()); });
		if (components.Empty())
		{
			return UPtr<T>();
		}
		return components.GetFirst();
	}

	void AddComponent(const UPtr<KSceneComponent>& component);
	void RemoveComponent(const UPtr<KSceneComponent>& component);

	void SerializeTo(nlohmann::json& json) const override;
	void DeserializeFrom(const nlohmann::json& json) override;

private:
	KtWindowViewport* viewport_;
	UPtr<TSceneObject> parent_;
	UPtr<KSceneComponent> rootComponent_;
	KtPool<UPtr<TSceneObject>> children_;
	KtPool<UPtr<KSceneComponent>> components_;
	size_t childrenIndex_;
};