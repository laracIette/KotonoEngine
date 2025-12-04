#pragma once
#include <type_traits>
#include <unordered_map>
#include <kotono_framework/Pool.h>
#include <kotono_framework/Average.h>
#include "Ptr.h"
#include "Guid.h"

class KObject;

template <class T>
concept Object = std::is_base_of_v<KObject, T>;

class SObjectManager final
{
	friend class SEngine;
	friend class SGame;

private:
	void Init();
	void Cleanup();

public:
	template <Object T, typename... Args> 
	UPtr<T> Create(Args... args)
	{
		auto* ptrOwner{ new UPtrOwner<T>() };
		auto* object{ new T(ptrOwner, args...) };
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
	KtPool<UPtrOwnerBase*> deletes_;

	UPtr<KObject> selectedObject_;

	void Register(KObject* object, UPtrOwnerBase* ptrOwner);

	void InitObjects();
	void DeleteObjects();

	void LogUPS() const;

	void OnMouseButtonLeftPressed();
};
