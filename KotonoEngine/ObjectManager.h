#pragma once
#include "Object.h"
#include <type_traits>
#include <unordered_set>
class KObjectManager
{
public:
	void Init();
	void Update();
	void Cleanup();

	template <class T> requires std::is_base_of_v<OObject, T>
	T* Create()
	{
		T* object = new T();
		Create(static_cast<OObject*>(object));
		return object;
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
