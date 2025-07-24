#include "InterfaceFloatTextBoxObject.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>
#include "InterfaceTextComponent.h"

RInterfaceFloatTextBoxObject::RInterfaceFloatTextBoxObject() :
	Base()
{
}

void RInterfaceFloatTextBoxObject::Init()
{
	Base::Init();

	SetIsEditable(true);
}

const float RInterfaceFloatTextBoxObject::GetValue() const
{
	return value_;
}

KtEvent<float>& RInterfaceFloatTextBoxObject::GetEventValueChanged()
{
	return eventValueChanged_;
}

void RInterfaceFloatTextBoxObject::SetValue(const float value)
{
	if (value == value_)
	{
		return;
	}

	const float delta = value - value_;
	value_ = value;
	eventValueChanged_.Broadcast(delta);
	GetTextComponent()->UpdateTextWithBinding();
}

void RInterfaceFloatTextBoxObject::OnMouseDown()
{
	Base::OnMouseDown();

	const float delta = Framework.GetInputManager().GetMouse().GetCursorPositionDelta().x;
	const float speedMultiplier = std::sqrt(std::abs(value_)) / 10.0f + 0.1f;
	SetValue(value_ + delta * speedMultiplier);
}
