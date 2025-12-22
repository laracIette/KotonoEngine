#pragma once
#include "Guid.h"
#include <kotono_common/Average.h>
#include <kotono_common/Pool.h>
#include <kotono_common/Ptr.h>
#include <type_traits>
#include <unordered_map>

class KObject;

class SObjectManager final
{
	friend class SCore;
	friend class SGame;

private:
	void Init();
	void Cleanup();

public:
	template <std::derived_from<KObject> T> 
	UPtr<T> Create()
	{
		auto* ptrOwner{ new UPtrOwner<T>() };
		auto* object{ new T(ptrOwner) };
		UPtr<T> ptr{ ptrOwner };
		Register(object, ptrOwner);
		return ptr;
	}

	void Delete(UPtrOwnerBase* ptrOwner);

	const UPtr<KObject>& SelectedObject() const;

private:
	void Quit();

	KtPool<UPtrOwnerBase*> inits_;
	KtPool<UPtrOwnerBase*> objects_;

	UPtr<KObject> selectedObject_;

	void Register(KObject* object, UPtrOwnerBase* ptrOwner);

	void InitObjects();

	void LogUPS() const;

	void OnMouseButtonLeftPressed();
};

inline SObjectManager ObjectManager;
