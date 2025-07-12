#include "InterfaceColliderComponent.h"
#include "Engine.h"
#include "ObjectManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/Viewport.h>
#include <kotono_framework/Collection.h>

void KInterfaceColliderComponent::Init()
{
	Base::Init();	

	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED).AddListener(KtDelegate<>(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonPressed));
	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED).AddListener(KtDelegate<>(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonReleased));
	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN).AddListener(KtDelegate<>(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonDown));
}

void KInterfaceColliderComponent::Update()
{
	Base::Update();

	UpdateOverlaps();
	BroadcastOverlaps();

	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED).RemoveListener(KtDelegate<>(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonPressed));
	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED).RemoveListener(KtDelegate<>(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonReleased));
	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate<>(this, &KInterfaceColliderComponent::OnEventMouseLeftButtonDown));
}

void KInterfaceColliderComponent::Cleanup()
{
	Base::Cleanup();
}

KtEvent<>& KInterfaceColliderComponent::GetEventPressed()
{
	return _eventPressed;
}

KtEvent<>& KInterfaceColliderComponent::GetEventReleased()
{
	return _eventReleased;
}

KtEvent<>& KInterfaceColliderComponent::GetEventDown()
{
	return _eventDown;
}

KtEvent<KInterfaceColliderComponent*>& KInterfaceColliderComponent::GetEventOverlap()
{
	return _eventOverlap;
}

const bool KInterfaceColliderComponent::GetIsMouseOverlapping() const
{
	const auto& cursorPosition = Framework.GetInputManager().GetMouse().GetCursorPosition();
	const auto viewportSize = glm::vec2(WindowViewport.GetExtent());
	const auto worldPosition = cursorPosition / viewportSize * 2.0f - glm::vec2(1.0f);
	return GetIsOverlapping(worldPosition);
}

void KInterfaceColliderComponent::UpdateOverlaps()
{
	const auto interfaceColliderComponents = Engine.GetObjectManager().GetAll<KInterfaceColliderComponent>();
	auto overlaps = KtCollection(interfaceColliderComponents.begin(), interfaceColliderComponents.end());
	overlaps.AddFilter([this](const KInterfaceColliderComponent* collider) { return collider != this; });
	overlaps.AddFilter([this](const KInterfaceColliderComponent* collider) { return GetIsOverlapping(collider); });
	_overlaps = overlaps.GetPool();
}

void KInterfaceColliderComponent::BroadcastOverlaps()
{
	for (auto* interfaceCollider : _overlaps)
	{
		_eventOverlap.Broadcast(interfaceCollider);
	}
}

void KInterfaceColliderComponent::OnEventMouseLeftButtonPressed()
{
	if (!GetIsMouseOverlapping())
	{
		return;
	}

	for (const auto* interfaceCollider : _overlaps)
	{
		if (interfaceCollider->GetVisibility() != EVisibility::None &&
			interfaceCollider->GetLayer() > GetLayer() && 
			interfaceCollider->GetIsMouseOverlapping())
		{
			return;
		}
	}

	_isPressed = true;
	_eventPressed.Broadcast();
}

void KInterfaceColliderComponent::OnEventMouseLeftButtonReleased()
{
	if (!_isPressed)
	{
		return;
	}

	_isPressed = false;

	if (GetIsMouseOverlapping())
	{
		_eventReleased.Broadcast();
	}
}

void KInterfaceColliderComponent::OnEventMouseLeftButtonDown()
{
	if (!_isPressed)
	{
		return;
	}

	_eventDown.Broadcast();
}
