#pragma once
#include "generated/InterfaceObject.generated.h"
#include "Object.h"
#include "CoordinateSpace.h"
#include <kotono_common/Collection.h>
#include <kotono_common/Pool.h>

class KtWindowViewport;
class KInterfaceComponent;

template <class T>
concept InterfaceComponent = std::is_base_of_v<KInterfaceComponent, T>;

class RInterfaceObject : public KObject
{
	GENERATED_RINTERFACEOBJECT()

public:
	RInterfaceObject(UPtrOwnerBase* ptrOwner);

protected:
	void Init() override;
	void Cleanup() override;

public:
	KtWindowViewport* GetViewport() const;
	const UPtr<RInterfaceObject>& GetParent() const;
	const UPtr<KInterfaceComponent>& RootComponent() const;
	const KtPool<UPtr<RInterfaceObject>>& GetChildren() const;

	void SetViewport(KtWindowViewport* viewport);
	void SetParent(const UPtr<RInterfaceObject>& parent, const ECoordinateSpace keepRect);

	/// Returns true if any component is hovered
	bool IsHovered() const;

	template <InterfaceComponent T>
	UPtr<T> GetComponent() const
	{
		auto components = KtCollection(components_.begin(), components_.end());
		components.AddFilter([](const UPtr<KInterfaceComponent>& component) { return dynamic_cast<T*>(component.Get()); });
		if (components.Empty())
		{
			return nullptr;
		}
		return components.GetFirst();
	}

	void AddComponent(const UPtr<KInterfaceComponent>& component);
	void RemoveComponent(const UPtr<KInterfaceComponent>& component);

private:
	KtWindowViewport* viewport_;
	UPtr<RInterfaceObject> parent_;
	UPtr<KInterfaceComponent> rootComponent_;
	KtPool<UPtr<RInterfaceObject>> children_;
	KtPool<UPtr<KInterfaceComponent>> components_;
	size_t childrenIndex_;
};

