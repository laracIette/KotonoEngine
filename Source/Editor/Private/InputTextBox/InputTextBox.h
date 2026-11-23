#pragma once
#include <Widget/Widget.h>

#include <InputHoldAction/InputHoldAction.h>

#include "InputTextBox.generated.h"

enum class EKey : u8;
enum class EInputState : u8;

class WInputTextBox : public WWidget
{
	GENERATED()

	using TextChangedCallback = std::function<void(std::string_view)>;
	using ValueToStringFunc = std::function<std::string()>;

public:
	WInputTextBox();

protected:
	auto Build() -> WidgetPtr override;

public:
	auto OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) -> b8 override;

	void SetActuationTime(f32 actuationTime);
	void SetRepeatTime(f32 repeatTime);

private:
	std::string text_;

    WritableProperty(b8, isEditable_, IsEditable);
	WritableProperty(TextChangedCallback, onTextChanged_, OnTextChanged);
	WritableProperty(ValueToStringFunc, valueToString_, ValueToString);

	ReadonlyProperty(f32, actuationTime_, ActuationTime);
	ReadonlyProperty(f32, repeatTime_, RepeatTime);

	UInputHoldAction holdAction_;
	char currentWriteCharacter_;
};