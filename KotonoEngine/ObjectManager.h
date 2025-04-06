#pragma once
#include "Object.h"
#include <type_traits>
#include <unordered_set>
#include <concepts>
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

	// Very slow function, potentially going through the whole list
	template <Object T> 
	T* GetFirst()
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
	
private:
	void Quit();

	std::unordered_set<OObject*> _objects;
	std::unordered_set<OObject*> _inits;

	void InitObjects();
	void UpdateObjects();
	void DeleteObjects();

	void Create(OObject* object);
};
