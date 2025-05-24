#pragma once
#include <type_traits>
#include <unordered_set>
#include <unordered_map>
#include <typeindex>
#include <kotono_framework/Event.h>

class KObject;

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
	const std::unordered_set<T*> GetAll() const
	{
		std::unordered_set<T*> result;
		const auto it = typeRegistry_.find(typeid(T));
		if (it != typeRegistry_.end())
		{
			const auto& objects = it->second;
			result.reserve(objects.size());

			for (KObject* obj : objects)
			{
				result.insert(static_cast<T*>(obj));
			}
		}
		return result;
	}
	
private:
	void Quit();

	std::unordered_set<KObject*> inits_;
	std::unordered_set<KObject*> objects_;

	std::unordered_map<std::type_index, std::unordered_set<KObject*>> typeRegistry_;

	KtEvent<> eventDrawSceneObjects_;
	KtEvent<> eventDrawSceneObjectWireframes_;
	KtEvent<> eventDrawInterfaceObjects_;
	KtEvent<> eventDrawInterfaceObjectBounds_;
	KtEvent<> eventDrawInterfaceObjectWireframes_;

	void InitObjects();
	void UpdateObjects();
	void DeleteObjects();
	void DrawObjects();
};
