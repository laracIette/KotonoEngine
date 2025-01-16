#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

void ObjectManager::Init()
{
	_lastUpdateTime = std::chrono::steady_clock::now();
}

void ObjectManager::Update(const float deltaTime)
{
	//InitObjects();
	UpdateObjects(deltaTime);
	DeleteObjects();
}

//void ObjectManager::InitObjects()
//{
//	for (Object* object : _inits)
//	{
//		object->Init();
//	}
//	_inits.clear();
//}

void ObjectManager::QueueInit(Object* object)
{
	_inits.insert(object);
}

Camera* ObjectManager::GetActiveCamera() const
{
	return nullptr;
}