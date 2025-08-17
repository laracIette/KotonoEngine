#pragma once
#include "Object.h"
#include "Visibility.h"
#include "CoordinateSpace.h"
#include <kotono_framework/Collection.h>
#include <kotono_framework/Pool.h>

class KtViewport;
class KSceneComponent; 

template <class T>
concept SceneComponent = std::is_base_of_v<KSceneComponent, T>;

class TSceneObject : public KObject
{
	BASECLASS(KObject)

public:
	TSceneObject();

protected:
	void Init() override;
	void Cleanup() override;

public:
	EVisibility GetVisibility() const;
	KtViewport* GetViewport() const;
	TSceneObject* GetParent() const;
	KSceneComponent* GetRootComponent() const;

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(TSceneObject* parent, const ECoordinateSpace keepTransform);

	template <SceneComponent T>
	T* GetComponent() const
	{
		auto components = KtCollection(components_.begin(), components_.end());
		components.AddFilter([](auto* component) { return dynamic_cast<T*>(component); });
		if (auto* component = components.GetFirst())
		{
			return static_cast<T*>(component);
		}
		return nullptr;
	}

	template <SceneComponent T>
	T* AddComponent()
	{
		T* component = new T(this);
		AddComponent(static_cast<KSceneComponent*>(component));
		return component;
	}

	void AddComponent(KSceneComponent* component);
	void RemoveComponent(const KSceneComponent* component);

	void SerializeTo(nlohmann::json& json) const override;
	void DeserializeFrom(const nlohmann::json& json) override;

private:
	EVisibility visibility_;
	KtViewport* viewport_;
	TSceneObject* parent_;
	KSceneComponent* rootComponent_;
	KtPool<TSceneObject*> children_;
	KtPool<KSceneComponent*> components_;
	size_t childrenIndex_;
};