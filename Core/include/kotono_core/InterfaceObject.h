#pragma once
#include "generated/InterfaceObject.generated.h"
#include "CoordinateSpace.h"
#include "Object.h"
#include <algorithm>
#include <kotono_common/Pool.h>

class KtWindowViewport;
class KInterfaceComponent;

class RInterfaceObject : public KObject
{
	GENERATED_RINTERFACEOBJECT()

private:
	friend class KInterface;

public:
	RInterfaceObject(UPtrOwnerBase* ptrOwner);

protected:
	void Cleanup() override;

	virtual void Init();
	virtual void Update(const float deltaTime);

public:
	bool GetCanUpdate() const;
	KtWindowViewport* GetViewport() const;
	UPtr<RInterfaceObject>& GetParent();
	UPtr<KInterfaceComponent>& RootComponent();
	const KtPool<UPtr<RInterfaceObject>>& GetChildren() const;

	void SetCanUpdate(const bool canUpdate);
	void SetViewport(KtWindowViewport* viewport);
	void SetParent(const UPtr<RInterfaceObject>& parent, const ECoordinateSpace keepRect);

	/// Returns true if any component is hovered
	bool IsHovered() const;

	template <std::derived_from<KInterfaceComponent> T>
	UPtr<T> GetComponent() const
	{
		return std::find_first_of(interfaceComponents_.begin(), interfaceComponents_.end(),
			[](const UPtr<KInterfaceComponent>& component) { return TryCast<T>(component); }
		);
	}

	void AddComponent(const UPtr<KInterfaceComponent>& component);
	void RemoveComponent(const UPtr<KInterfaceComponent>& component);

	virtual void Spawn();

private:
	void InitInterfaceComponents();
	void UpdateInterfaceComponents(const float deltaTime);

private:
	bool isInit_;
	bool canUpdate_;
	KtWindowViewport* viewport_;
	UPtr<RInterfaceObject> parent_;
	UPtr<KInterfaceComponent> rootComponent_;
	KtPool<UPtr<RInterfaceObject>> children_;
	KtPool<UPtr<KInterfaceComponent>> interfaceComponents_;
	size_t childrenIndex_;
};

