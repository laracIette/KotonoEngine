#pragma once
#include <kotono_common/Pool.h>
class KInterfaceColliderComponent;
class GInterfacePhysicsManager final
{
	friend class GCore;

private:
	void Update();

public:
	void Register(KInterfaceColliderComponent* colliderComponent);
	void Unregister(KInterfaceColliderComponent* colliderComponent);

private:
	UPool<KInterfaceColliderComponent*> colliderComponents_;
};

inline GInterfacePhysicsManager InterfacePhysicsManager;
