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
	void Update();
	void Cleanup();

public:
	template <Object T, typename... Args> 
	UPtr<T> Create(Args... args)
	{
		auto* ptrOwner{ new UPtrOwner<T>() };
		auto* object{ new T(ptrOwner, args...) };
		UPtr<T> ptr{ ptrOwner };
		Register(object, ptrOwner, ptr);
		return ptr;
	}

	void Delete(UPtrOwnerBase* ptrOwner);

	uint64_t GetCurrentUpdate() const;

	const UPtr<KObject>& SelectedObject() const;

	UPtr<KObject> Get(const UGuid& guid) const;

private:
	void Quit();

	KtPool<UPtrOwnerBase*> inits_;
	KtPool<UPtrOwnerBase*> objects_;
	KtPool<UPtrOwnerBase*> deletes_;

	std::unordered_map<UGuid, UPtr<KObject>> register_;

	UPtr<KObject> selectedObject_;

	uint64_t currentUpdate_;

	void Register(KObject* object, UPtrOwnerBase* ptrOwner, const UPtr<KObject>& ptr);

	void InitObjects();
	void UpdateObjects();
	void DeleteObjects();

	void LogUPS() const;

	void OnMouseButtonLeftPressed();
};
