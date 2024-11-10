#include "ObjectManager.h"
#include "FIleExplorerWindowComponent.h"
#include <iostream>
#include "Drawable.h"
#include "Drawable3D.h"
#include "Image.h"
#include "Material.h"


ObjectManager::ObjectManager() :
	_lastUpdateTime(std::chrono::steady_clock::now()),
	_fileExplorer(new FileExplorer()),
	_gameState(GameState::Stopped)
{
	// Open project
	_fileExplorer->SetDirectoryPath(R"(C:\Users\nicos\Documents\Kotono Projects\Test)");

	// Initialize all WindowComponents
	auto fileExplorerWindowComponent = new FileExplorerWindowComponent(_fileExplorer);



	auto material = new Material();
	material->Compile();

	auto texture = new ImageTexture(R"(Assets\Default\Textures\image.jpg)");

	auto image = new Image();
	image->SetMaterial(material);
	image->SetTexture(texture);

	Create(image);
}

ObjectManager::~ObjectManager()
{
	for (Object* object : _objects)
	{
		delete object;
	}

	delete _fileExplorer;
}

void ObjectManager::Update()
{
	if (_gameState == GameState::Playing)
	{
		InitObjects();
		UpdateObjects();
		DeleteObjects();
	}
}

void ObjectManager::InitObjects()
{
	for (Object* object : _inits)
	{
		object->Init();
	}
	_inits.clear();
}

void ObjectManager::UpdateObjects()
{
	auto now = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration<float>(now - _lastUpdateTime);
	float deltaTime = duration.count();
	_lastUpdateTime = now;

	for (Object* object : _objects)
	{
		if (object->GetIsUpdate())
		{
			object->Update(deltaTime);
		}
	}
}

void ObjectManager::DeleteObjects()
{
	for (auto it = _objects.begin(); it != _objects.end(); )
	{
		Object* object = *it;
		if (object->GetIsDelete())
		{
			// Delete the object pointed to by the iterator
			Delete(object);

			// Erase returns the next iterator
			it = _objects.erase(it); 
		}
		else
		{
			// Only increment if we didn't erase
			++it; 
		}
	}
}

void ObjectManager::Delete(Object* object) const
{
	// If the object is a drawable
	if (auto* objectAsDrawable2D = dynamic_cast<Drawable2D*>(object))
	{
		for (auto* drawable2D : GetObjectsOfType<Drawable2D>())
		{
			if (drawable2D->GetParent() == object)
			{
				drawable2D->SetParent(objectAsDrawable2D->GetParent());
			}
		}
	}
	else if (auto* objectAsDrawable3D = dynamic_cast<Drawable3D*>(object))
	{
		for (auto* drawable3D : GetObjectsOfType<Drawable3D>())
		{
			if (drawable3D->GetParent() == object)
			{
				drawable3D->SetParent(objectAsDrawable3D->GetParent());
			}
		}
	}
	
	delete object;
}

void ObjectManager::Draw() const
{
	for (auto* drawable : GetObjectsOfType<Drawable>())
	{
		if (drawable->GetIsDraw())
		{
			drawable->UpdateShader();
			drawable->Draw();
		}
	}
}

void ObjectManager::Create(Object* object)
{
	_objects.insert(object);
	_inits.insert(object);
}

template <typename T>
const std::vector<T*> ObjectManager::GetObjectsOfType() const
{
	// Ensure that T is derived from Object
	static_assert(std::is_base_of<Object, T>::value, "T must derive from Object");

	std::vector<T*> result;
	for (auto* obj : _objects)
	{
		if (T* casted = dynamic_cast<T*>(obj))
		{
			result.push_back(casted);
		}
	}
	return result;
}