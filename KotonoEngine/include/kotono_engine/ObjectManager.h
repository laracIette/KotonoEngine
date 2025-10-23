#pragma once
#include <type_traits>
#include <kotono_framework/Pool.h>
#include <kotono_framework/AverageTime.h>
#include "Ptr.h"

class KObject;

template <class T>
concept Object = std::is_base_of_v<KObject, T>;

class SObjectManager final
{
	friend class SEngine;

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
		Register(object, ptrOwner);
		return ptrOwner;
	}

	void Delete(UPtrOwnerBase* ptrOwner);

	float GetAverageUpdateTime() const;
	float GetAverageDrawTime() const;

	int64_t GetCurrentUpdate() const;

	const UPtr<KObject>& SelectedObject() const;

private:
	void Quit();

	KtPool<UPtrOwnerBase*> inits_;
	KtPool<UPtrOwnerBase*> objects_;
	KtPool<UPtrOwnerBase*> deletes_;

	UPtr<KObject> selectedObject_;

	KtAverageTime<256> updateAverageTime_;
	KtAverageTime<64> drawAverageTime_;

	int64_t currentUpdate_;

	bool canDraw_;

	void Register(KObject* object, UPtrOwnerBase* ptrOwner);

	void InitObjects();
	void UpdateObjects();
	void DeleteObjects();
	void SubmitDrawObjects();

	void LogUPS() const;

	void OnMouseButtonLeftPressed();
};
