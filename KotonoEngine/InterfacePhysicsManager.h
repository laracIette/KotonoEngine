#pragma once
#include <kotono_framework/Pool.h>
class KInterfaceColliderComponent;
class SInterfacePhysicsManager final
{
	friend class SEngine;

private:
	void Update();

public:
	void Register(KInterfaceColliderComponent* colliderComponent);
	void Unregister(KInterfaceColliderComponent* colliderComponent);

private:
	KtPool<KInterfaceColliderComponent*> colliderComponents_;
};

