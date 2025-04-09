#pragma once
#include "Object.h"
#include <type_traits>
#include <unordered_set>
#include <vector>
#include <concepts>
#include <unordered_map>
#include <typeindex>
template <class T>
concept Object = std::is_base_of_v<OObject, T>;
class KObjectManager
{
public:
	void Init();
	void Update();
	void Cleanup();

	template <Object T> 
	T* Create()
	{
		T* object = new T();
		Create(static_cast<OObject*>(object));
		return object;
	}

	// Very slow function, potentially going through the whole list, performing dynamic cast
	template <Object T> 
	T* GetFirstOfType()
	{
		for (const OObject* object : _objects)
		{
			if (T* casted = dynamic_cast<T*>(object))
			{
				return casted;
			}
		}
		return nullptr;
	}

	// Slow function, going through the whole list
	template <Object T> 
	const std::unordered_set<T*> GetAllOfType()
	{
		std::unordered_set<T*> result;
		const auto& objects = _typeRegistry[typeid(T)];
		result.reserve(objects.size());
		for (OObject* obj : objects)
		{
			result.insert(static_cast<T*>(obj));
		}
		return result;
	}
	
private:
	void Quit();

	std::unordered_set<OObject*> _objects;
	std::unordered_set<OObject*> _inits;

	std::unordered_map<std::type_index, std::unordered_set<OObject*>> _typeRegistry;

	void InitObjects();
	void UpdateObjects();
	void DeleteObjects();

	void Create(OObject* object);
};
