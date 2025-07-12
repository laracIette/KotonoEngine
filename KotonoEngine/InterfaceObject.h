#pragma once
#include "Object.h"
#include "CoordinateSpace.h"
#include "Visibility.h"
#include <kotono_framework/Collection.h>
#include <kotono_framework/Pool.h>

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
	const KtPool<RInterfaceObject*>& GetChildren() const;

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(RInterfaceObject* parent, const ECoordinateSpace keepRect);

	template <InterfaceComponent T>
	T* GetComponent() const
	{
		auto components = KtCollection(components_.begin(), components_.end());
		components.AddFilter([](const KInterfaceComponent* component) { return dynamic_cast<const T*>(component); });
		if (auto* component = components.GetFirst())
		{
			return static_cast<T*>(component);
		}
		return nullptr;
	}

	template <InterfaceComponent T>
	T* AddComponent()
	{
		T* component = new T(this);
		AddComponent(static_cast<KInterfaceComponent*>(component));
		return component;
	}

	void AddComponent(KInterfaceComponent* component);
	void RemoveComponent(KInterfaceComponent* component);

private:
	EVisibility visibility_;
	KtViewport* viewport_;
	RInterfaceObject* parent_;
	KInterfaceComponent* rootComponent_;
	KtPool<RInterfaceObject*> children_;
	KtPool<KInterfaceComponent*> components_;
	size_t childrenIndex_;
};

