#pragma once
#include <kotono_framework/Pool.h>
class KInterfaceColliderComponent;
class SInterfacePhysicsManager final
{
public:
	void Update();

	void Register(KInterfaceColliderComponent* colliderComponent);
	void Unregister(KInterfaceColliderComponent* colliderComponent);

private:
	KtPool<KInterfaceColliderComponent*> colliderComponents_;
};

