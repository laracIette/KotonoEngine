#pragma once
#include "Object.h"
#include <type_traits>
#include <unordered_set>
#include <vector>
#include <concepts>
#include <unordered_map>
#include <typeindex>
#include <kotono_framework/Event.h>
template <class T>
concept Object = std::is_base_of_v<OObject, T>;
class KObjectManager final
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

	template <Object T> 
	T* Create()
	{
		T* object = new T();
		Create(static_cast<OObject*>(object));
		return object;
	}

	template <Object T> 
	T* GetFirstOfType() const
	{
		const auto it = _typeRegistry.find(typeid(T));
		if (it != _typeRegistry.end())
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
	const std::unordered_set<T*> GetAllOfType() const
	{
		std::unordered_set<T*> result;
		const auto it = _typeRegistry.find(typeid(T));
		if (it != _typeRegistry.end())
		{
			const auto& objects = it->second;
			result.reserve(objects.size());

			for (OObject* obj : objects)
			{
				result.insert(static_cast<T*>(obj));
			}
		}
		return result;
	}
	
private:
	void Quit();

	std::unordered_set<OObject*> _objects;
	std::unordered_set<OObject*> _inits;

	std::unordered_map<std::type_index, std::unordered_set<OObject*>> _typeRegistry;

	KtEvent<> _eventDrawSceneObjects;
	KtEvent<> _eventDrawSceneObjectWireframes;
	KtEvent<> _eventDrawInterfaceObjects;
	KtEvent<> _eventDrawInterfaceObjectBounds;
	KtEvent<> _eventDrawInterfaceObjectWireframes;

	void InitObjects();
	void UpdateObjects();
	void DeleteObjects();
	void DrawObjects();

	void Create(OObject* object);
};
