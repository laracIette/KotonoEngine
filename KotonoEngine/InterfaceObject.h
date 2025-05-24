#pragma once
#include "Object.h"
#include "Rect.h"
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

	const URect& GetRect() const;
	URect& GetRect();
	const EVisibility GetVisibility() const;
	KtViewport* GetViewport() const; // todo: move that to a renderable interface object component ?
	RInterfaceObject* GetParent() const;
	RInterfaceObject* GetRoot();
	const int32_t GetLayer() const;
	const std::unordered_set<RInterfaceObject*>& GetChildren() const;

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(RInterfaceObject* parent, const ECoordinateSpace keepRect);
	void SetLayer(const int32_t layer);

	template <InterfaceComponent T>
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

	template <InterfaceComponent T>
	T* AddComponent()
	{
		T* component = AddObject<T>(this);
		AddComponent(static_cast<KInterfaceComponent*>(component));
		return component;
	}

private:
	URect rect_;
	EVisibility visibility_;
	KtViewport* viewport_;
	RInterfaceObject* parent_;
	std::unordered_set<RInterfaceObject*> children_;
	int32_t layer_;
	std::unordered_set<KInterfaceComponent*> components_;

	void AddComponent(KInterfaceComponent* component);
};

