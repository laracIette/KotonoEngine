#pragma once
#include <unordered_set>
#include <chrono>
#include "Object.h"
#include "FileExplorer.h"
#include "GameState.h"
#include "Camera.h"
#include "Renderer.h"
class ObjectManager
{
public:
	ObjectManager();

	void Init();
	void Update(const float deltaTime);
	virtual void Draw() = 0;

	Camera* GetActiveCamera() const;

protected:
	void QueueInit(Object* object);
	virtual void UpdateObjects(const float deltaTime) = 0;
	virtual void DeleteObjects() = 0;

private:
	std::unordered_set<Object*> _inits;

	std::chrono::steady_clock::time_point _lastUpdateTime;

	//void InitObjects();
};
