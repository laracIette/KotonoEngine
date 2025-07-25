#pragma once
#include <type_traits>
#include <unordered_set>
#include <unordered_map>
#include <typeindex>
#include <kotono_framework/Pool.h>
#include <array>

class KObject;

template <class T>
concept Object = std::is_base_of_v<KObject, T>;

class SObjectManager final
{
public:
	void Init();
	void Update();
	void Cleanup();

	void Register(KObject* object);
	void Delete(KObject* object);

	template <Object T, typename... Args> 
	T* Create(Args... args)
	{
		T* object = new T(args...);
		Register(object);
		return object;
	}

	template <Object T> 
	T* GetFirst() const
	{
		const auto it = typeRegistry_.find(typeid(T));
		if (it != typeRegistry_.end())
		{
			const auto& objects = it->second;
			if (!objects.empty())
			{
				return static_cast<T*>(objects[0]);
			}
		}
		return nullptr;
	}

	template <Object T> 
	const std::vector<T*> GetAll() const
	{
		std::vector<T*> result{};

		const auto it = typeRegistry_.find(typeid(T));
		if (it != typeRegistry_.end())
		{
			const auto& objects = it->second;

			result.reserve(objects.size());
			for (auto* object : objects)
			{
				result.push_back(static_cast<T*>(object));
			}
		}
		return result;
	}

	const float GetAverageUpdateTime() const;
	const float GetAverageDrawTime() const;
	
private:
	void Quit();

	KtPool<KObject*> inits_;
	KtPool<KObject*> objects_;
	KtPool<KObject*> deletes_;

	std::unordered_map<std::type_index, std::unordered_set<KObject*>> typeRegistry_;

	std::array<float, 256> updateTimes_;
	size_t updateTimeIndex_;
	float updateTimesSum_;

	std::array<float, 64> drawTimes_;
	size_t drawTimeIndex_;
	float drawTimesSum_;

	bool canDraw_;

	void InitObjects();
	void UpdateObjects();
	void DeleteObjects();
	void SubmitDrawObjects();

	void LogUPS() const;
};
