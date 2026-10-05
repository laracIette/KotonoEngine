#pragma once
#include "InputTextBox.generated.h"
#include <Widget/Widget.h>

#include <InputHoldAction/InputHoldAction.h>

enum class EKey : u8;
enum class EInputState : u8;

class WInputTextBox : public WWidget
{
	GENERATED_WINPUTTEXTBOX()

	using TextChangedCallback = std::function<void(std::string_view)>;
	using ValueToStringFunc = std::function<std::string()>;

public:
	WInputTextBox();

protected:
	WidgetPtr Build() override;

public:
	b8 OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) override;

public:
	void SetActuationTime(f32 actuationTime);
	void SetRepeatTime(f32 repeatTime);

private:
	std::string text_;

	WritableProperty(TextChangedCallback, onTextChanged_, OnTextChanged);
	WritableProperty(ValueToStringFunc, valueToString_, ValueToString);

	ReadonlyProperty(f32, actuationTime_, ActuationTime, Value);
	ReadonlyProperty(f32, repeatTime_, RepeatTime, Value);

	UInputHoldAction holdAction_;
	char currentWriteCharacter_;
};