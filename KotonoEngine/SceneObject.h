#pragma once
#include "Object.h"
#include "Transform.h"
#include "Visibility.h"
#include <kotono_framework/Collection.h>

class KtViewport;
class KSceneComponent; 

template <class T>
concept SceneComponent = std::is_base_of_v<KSceneComponent, T>;

class TSceneObject : public KObject
{
	BASECLASS(KObject)

public:
	void Init() override;

	const UTransform& GetTransform() const;
	UTransform& GetTransform();
	const EVisibility GetVisibility() const;
	KtViewport* GetViewport() const;
	TSceneObject* GetParent() const;

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(TSceneObject* parent, const ECoordinateSpace keepTransform);

	void SerializeTo(nlohmann::json& json) const override;
	void DeserializeFrom(const nlohmann::json& json) override;

	template <SceneComponent T>
	T* GetComponent() const
	{
		auto components = KtCollection(components_);
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
		T* component = AddObject<T>(this);
		AddComponent(static_cast<KSceneComponent*>(component));
		return component;
	}

private:
	UTransform transform_;
	EVisibility visibility_;
	KtViewport* viewport_;
	TSceneObject* parent_;
	std::unordered_set<TSceneObject*> children_;
	std::unordered_set<KSceneComponent*> components_;

	void AddComponent(KSceneComponent* component);
};