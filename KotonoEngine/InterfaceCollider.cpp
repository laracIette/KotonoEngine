#include "InterfaceCollider.h"
#include "Engine.h"
#include "ObjectManager.h"

void RInterfaceCollider::Init()
{
	Base::Init();	
}

void RInterfaceCollider::Update()
{
	Base::Update();
	UpdateOverlaps();
	BroadcastOverlaps();
}

void RInterfaceCollider::Cleanup()
{
	Base::Cleanup();
}

KtEvent<RInterfaceCollider*>& RInterfaceCollider::GetEventOverlap()
{
	return _eventOverlap;
}

void RInterfaceCollider::UpdateOverlaps()
{
	auto interfaceColliders = Engine.GetObjectManager().GetAllOfType<RInterfaceCollider>();
	interfaceColliders.erase(this);

	_overlaps.clear();

	for (auto* interfaceCollider : interfaceColliders)
	{
		if (GetRect().GetIsOverlapping(interfaceCollider->GetRect()))
		{
			_overlaps.insert(interfaceCollider);
		}
	}
}

void RInterfaceCollider::BroadcastOverlaps()
{
	for (auto* interfaceCollider : _overlaps)
	{
		_eventOverlap.Broadcast(interfaceCollider);
	}
}