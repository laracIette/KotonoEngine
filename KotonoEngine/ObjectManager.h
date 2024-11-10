#pragma once
#include <unordered_set>
#include <chrono>
#include "Object.h"
#include "Drawable2D.h"
#include "Drawable3D.h"
#include "FileExplorer.h"
#include "Shader.h"
#include "GameState.h"
class ObjectManager final
{
public:
	ObjectManager();
	~ObjectManager();

	void Create(Object* object);

	void Update();
	void Draw() const;

private:
	std::unordered_set<Object*> _objects;
	std::unordered_set<Object*> _inits;

	std::chrono::steady_clock::time_point _lastUpdateTime;

	FileExplorer* _fileExplorer;

	GameState _gameState;

	void UpdateObjects();
	void InitObjects();
	void DeleteObjects();

	void Delete(Object* object) const;

	template<typename T>
	const std::vector<T*> GetObjectsOfType() const;
};

