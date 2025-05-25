#include "InterfaceFloatTextBoxObject.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>
#include "InterfaceTextComponent.h"
#include "Timer.h"

void RInterfaceFloatTextBoxObject::Init()
{
	Base::Init();

	SetIsEditable(true);

	GetTextComponent()->SetTextBinding([this]() { return std::to_string(value_); });
	GetTextComponent()->GetUpdateTimer()->SetDuration(1.0f / 60.0f);
	GetTextComponent()->GetUpdateTimer()->SetIsRepeat(true);
	GetTextComponent()->GetUpdateTimer()->Start();

	// todo: looks like duplicate with value changed event
}

const float RInterfaceFloatTextBoxObject::GetValue() const
{
	return value_;
}

KtEvent<float>& RInterfaceFloatTextBoxObject::GetValueChangedEvent()
{
	return valueChangedEvent_;
}

void RInterfaceFloatTextBoxObject::SetValue(const float value)
{
	const float delta = value - value_;
	value_ = value;
	valueChangedEvent_.Broadcast(delta);
}

void RInterfaceFloatTextBoxObject::OnMouseDown()
{
	Base::OnMouseDown();

	const float delta = Framework.GetInputManager().GetMouse().GetCursorPositionDelta().x;
	const float speedMultiplier = std::sqrt(std::abs(value_)) / 10.0f + 0.1f;
	SetValue(value_ + delta * speedMultiplier);
}
