#pragma once
#include <kotono_common/Pool.h>
class KInterfaceColliderComponent;
class SInterfacePhysicsManager final
{
	friend class SCore;

private:
	void Update();

public:
	void Register(KInterfaceColliderComponent* colliderComponent);
	void Unregister(KInterfaceColliderComponent* colliderComponent);

private:
	UPool<KInterfaceColliderComponent*> colliderComponents_;
};

inline SInterfacePhysicsManager InterfacePhysicsManager;
