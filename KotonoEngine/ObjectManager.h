#pragma once
#include <type_traits>
#include <unordered_set>
#include <unordered_map>
#include <typeindex>
#include <kotono_framework/Event.h>
#include <kotono_framework/Pool.h>
#include <array>
#include <any>

class KObject;
class KTimer;

template <class T>
concept Object = std::is_base_of_v<KObject, T>;

class SObjectManager final
{
public:
	void Init();
	void Update();
	void Cleanup();

	KtEvent<>& GetEventDrawSceneObjects();
	KtEvent<>& GetEventDrawSceneObjectWireframes();
	KtEvent<>& GetEventDrawInterfaceObjects();
	KtEvent<>& GetEventDrawInterfaceObjectBounds();
	KtEvent<>& GetEventDrawInterfaceObjectWireframes();

	void Register(KObject* object);
	void Delete(KObject* object);

	template <Object T, typename... Args> 
	T* Create(Args... args)
	{
		T* object = new T(args...);
		Register(static_cast<KObject*>(object));
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
	
private:
	void Quit();

	KtPool<KObject*> inits_;
	KtPool<KObject*> objects_;
	KtPool<KObject*> deletes_;

	std::unordered_map<std::type_index, std::unordered_set<KObject*>> typeRegistry_;

	KtEvent<> eventDrawSceneObjects_;
	KtEvent<> eventDrawSceneObjectWireframes_;
	KtEvent<> eventDrawInterfaceObjects_;
	KtEvent<> eventDrawInterfaceObjectBounds_;
	KtEvent<> eventDrawInterfaceObjectWireframes_;

	std::array<float, 60> updateTimes_;
	size_t updateTimeIndex_;
	float updateTimesSum_;
	float updateTimesAverage_;

	KTimer* drawTimer_;
	bool canDraw_;

	void InitObjects();
	void UpdateObjects();
	void DeleteObjects();
	void DrawObjects();
	void SubmitDrawObjects();

	void LogUPS();
};
