#pragma once
#include <unordered_set>
#include <chrono>
#include "Object.h"
#include "FileExplorer.h"
#include "GameState.h"
class ObjectManager final
{
public:
	ObjectManager();
	~ObjectManager();

	void Init();
	void Update();
	void Draw() const;

	void Create(Object* object);

private:
	std::unordered_set<Object*> _objects;
	std::unordered_set<Object*> _inits;

	std::chrono::steady_clock::time_point _lastUpdateTime;

	GameState _gameState;

	void UpdateObjects();
	void InitObjects();
	void DeleteObjects();

	void Delete(Object* object) const;

	void SetGameState(const GameState& gameState);

	template<typename T>
	const std::vector<T*> GetObjectsOfType() const;
};
