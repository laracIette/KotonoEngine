#pragma once
#include <type_traits>
#include <kotono_framework/Pool.h>
#include <kotono_framework/AverageTime.h>
#include <array>

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
	void Register(KObject* object);
	void Delete(KObject* object);

	template <Object T, typename... Args> 
	T* Create(Args... args)
	{
		T* object = new T(args...);
		Register(object);
		return object;
	}

	float GetAverageUpdateTime() const;
	float GetAverageDrawTime() const;

	int64_t GetCurrentUpdate() const;
	
private:
	void Quit();

	KtPool<KObject*> inits_;
	KtPool<KObject*> objects_;
	KtPool<KObject*> deletes_;

	KtAverageTime<256> updateAverageTime_;
	KtAverageTime<64> drawAverageTime_;

	int64_t currentUpdate_;

	bool canDraw_;

	void InitObjects();
	void UpdateObjects();
	void DeleteObjects();
	void SubmitDrawObjects();

	void LogUPS() const;
};
