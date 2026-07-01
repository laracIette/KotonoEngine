#include "InterfacePhysicsManager.h"
#include "InterfaceColliderComponent.h"

void GInterfacePhysicsManager::Update()
{
	//for (size i{ 0 }; i < colliderComponents_.size(); ++i)
	//{
	//	auto* current{ colliderComponents_[i] };
	//	for (size j{ i + 1 }; j < colliderComponents_.size(); ++j)
	//	{
	//		auto* other{ colliderComponents_[j] };
	//		if (current->GetIsOverlapping(other))
	//		{
	//			current->eventOverlap_.Broadcast(other);
	//			other->eventOverlap_.Broadcast(current);
	//		}
	//	}
	//}
}

void GInterfacePhysicsManager::Register(KInterfaceColliderComponent* colliderComponent)
{
	colliderComponents_.Add(colliderComponent);
	colliderComponent->physicsIndex_ = colliderComponents_.LastIndex();
}

void GInterfacePhysicsManager::Unregister(KInterfaceColliderComponent* colliderComponent)
{
	const size index{ colliderComponent->physicsIndex_ };
	if (colliderComponents_.RemoveAt(index) == EPoolRemoveResult::ItemSwappedAndRemoved)
	{
		colliderComponents_[index]->physicsIndex_ = index;
	}
}
