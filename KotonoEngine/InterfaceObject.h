#pragma once
#include "Object.h"
#include "CoordinateSpace.h"
#include "Visibility.h"
#include <kotono_framework/Collection.h>

class KtViewport;
class KInterfaceComponent;

template <class T>
concept InterfaceComponent = std::is_base_of_v<KInterfaceComponent, T>;

class RInterfaceObject : public KObject
{
	BASECLASS(KObject)

public:
	void Construct() override;
	void Init() override;
	void Update() override;
	void Cleanup() override;

	const EVisibility GetVisibility() const;
	KtViewport* GetViewport() const;
	RInterfaceObject* GetParent() const;
	KInterfaceComponent* GetRootComponent() const;
	const int32_t GetLayer() const;
	const std::unordered_set<RInterfaceObject*>& GetChildren() const;

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(RInterfaceObject* parent, const ECoordinateSpace keepRect);
	void SetLayer(const int32_t layer);

	template <InterfaceComponent T>
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

	template <InterfaceComponent T>
	T* AddComponent()
	{
		T* component = AddObject<T>(this);
		AddComponent(static_cast<KInterfaceComponent*>(component));
		return component;
	}

private:
	EVisibility visibility_;
	KtViewport* viewport_; // todo: maybe move to component ????
	RInterfaceObject* parent_;
	int32_t layer_;
	KInterfaceComponent* rootComponent_;
	std::unordered_set<RInterfaceObject*> children_;
	std::unordered_set<KInterfaceComponent*> components_;

	void AddComponent(KInterfaceComponent* component);
};

