#include "ObjectManager.h"
#include "FileExplorerComponent.h"
#include "Drawable.h"
#include "Drawable2D.h"
#include "Drawable3D.h"
#include "Image.h"
#include "Material.h"
#include <magic_enum/magic_enum.hpp>

ObjectManager* ObjectManagerInstance = new ObjectManager();

ObjectManager::ObjectManager() :
	_lastUpdateTime(std::chrono::steady_clock::now()),
	_gameState(GameState::Stopped)
{
}

ObjectManager::~ObjectManager()
{
	std::cout << "ObjectManager::~ObjectManager(): deleting objects..." << std::endl;

	for (Object* object : _objects)
	{
		delete object;
	}

	std::cout << "ObjectManager::~ObjectManager(): finished deleting objects." << std::endl;
}

void ObjectManager::Init()
{
	// Initialize all WindowComponents
	auto fileExplorerComponent = new FileExplorerComponent();

	auto material = new Material();
	material->Compile();

	auto texture = new ImageTexture(R"(Assets\Default\Textures\image.jpg)");

	auto image = new Image();
	image->SetMaterial(material);
	image->SetTexture(texture);

	image->GetRect()->SetRelativeSize(glm::vec2(100.0f, 100.0f));
	image->GetRect()->SetRelativePosition(glm::vec2(100.0f, 100.0f));
	image->GetRect()->SetRelativePositionVelocity(glm::vec2(10.0f, 5.0f));

	SetGameState(GameState::Playing);
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

void ObjectManager::SetGameState(const GameState& gameState)
{
	_gameState = gameState;
	std::cout << "ObjectManager::SetGameState(const GameState&): _gameState set to " << magic_enum::enum_name(gameState) << '.' << std::endl;
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