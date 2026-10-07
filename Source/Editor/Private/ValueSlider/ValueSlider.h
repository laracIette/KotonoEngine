#pragma once
#include <Widget/Widget.h>

#include <conversion_utils.h>

#include "ValueSlider.generated.h"

class WValueSlider : public WWidget
{
	GENERATED()

public:
	using ValueToStringFunc = std::function<std::string()>;
	using TextChangedCallback = std::function<void(std::string_view)>;
	using SlideCallback = std::function<void(f32)>;

protected:
	WidgetPtr Build() override;
	
private:
	WritableProperty(ValueToStringFunc, valueToString_, ValueToString);
	WritableProperty(TextChangedCallback, onTextChanged_, OnTextChanged);
	WritableProperty(SlideCallback, onSlide_, OnSlide);

public:
	template <std::floating_point T>
	static auto FromPointer(T* value) -> WidgetPtr
	{
		Check(Abort, value, "value is null");
		return (
			UCreate<WValueSlider>{}()
			| Apply(&WValueSlider::SetValueToString, [value]() { return to_string<T>(*value); })
			| Apply(&WValueSlider::SetOnTextChanged, [value](std::string_view text) { *value = from_string<T>(text); })
			| Apply(&WValueSlider::SetOnSlide, [value](f32 delta){ *value += delta * std::max(0.01f, std::abs(*value * 0.01f)); })
		);
	}

	template <std::integral T>
	static auto FromPointer(T* value) -> WidgetPtr
	{
		Check(Abort, value, "value is null");
		return (
			UCreate<WValueSlider>{}()
			| Apply(&WValueSlider::SetValueToString, [value]() { return to_string(*value); })
			| Apply(&WValueSlider::SetOnTextChanged, [value](std::string_view text) { *value = from_string<T>(text); })
			| Apply(&WValueSlider::SetOnSlide, [value](f32 delta) { *value += delta; })
		);
	}
};