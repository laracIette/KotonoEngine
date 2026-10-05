#pragma once
#include <Widget/Widget.h>

#include "ValueBox.generated.h"

template <typename T>
concept StringCompatible = std::convertible_to<const T&, std::string> && std::assignable_from<T&, std::string_view>;

class WValueBox : public WWidget
{
	GENERATED()

public:
	using ValueToStringFunc = std::function<std::string()>;
	using TextChangedCallback = std::function<void(std::string_view)>;

protected:
	WidgetPtr Build() override;

public:
	static auto FromPointer(StringCompatible auto* value) -> WidgetPtr
	{
		assert(value != nullptr);
		return (
			UCreate<WValueBox>{}()
			| Apply(&WValueBox::SetValueToString, [value]() { return *value; })
			| Apply(&WValueBox::SetOnTextChanged, [value](std::string_view text) { *value = text; })
		);
	}

private:
	WritableProperty(ValueToStringFunc, valueToString_, ValueToString);
	WritableProperty(TextChangedCallback, onTextChanged_, OnTextChanged);
};