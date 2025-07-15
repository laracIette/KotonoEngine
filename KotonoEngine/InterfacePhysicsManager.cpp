#include "InterfacePhysicsManager.h"
#include "InterfaceColliderComponent.h"

void SInterfacePhysicsManager::Update()
{
	for (size_t i = 0; i < colliderComponents_.Size(); ++i)
	{
		auto* current = colliderComponents_[i];
		for (size_t j = i + 1; j < colliderComponents_.Size(); ++j)
		{
			auto* other = colliderComponents_[j];
			if (current->GetIsOverlapping(other))
			{
				current->eventOverlap_.Broadcast(other);
				other->eventOverlap_.Broadcast(current);
			}
		}
	}
}

void SInterfacePhysicsManager::Register(KInterfaceColliderComponent* colliderComponent)
{
	colliderComponents_.Add(colliderComponent);
	colliderComponent->physicsIndex_ = colliderComponents_.LastIndex();
}

void SInterfacePhysicsManager::Unregister(KInterfaceColliderComponent* colliderComponent)
{
	const size_t index = colliderComponent->physicsIndex_;
	colliderComponents_.RemoveAt(index);
	if (index < colliderComponents_.Size())
	{
		colliderComponents_[index]->physicsIndex_ = index;
	}
}
