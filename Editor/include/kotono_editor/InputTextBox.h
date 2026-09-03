#pragma once
#include "generated/InputTextBox.generated.h"
#include <kotono_core/Widget.h>

#include <kotono_input/InputHoldAction.h>
enum class EKey : u8;
enum class EInputState : u8;
class WInputTextBox : public WWidget
{
	GENERATED_WINPUTTEXTBOX()

	using TextChangedFunction = std::function<void(std::string const&)>;

public:
	WInputTextBox();

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

public:
	std::string_view GetText() const;
	TextChangedFunction const& GetOnTextChanged() const;
	f32 GetActuationTime() const;
	f32 GetRepeatTime() const;

	void SetText(std::string_view text);
	void SetOnTextChanged(TextChangedFunction const& onTextChanged);
	void SetActuationTime(f32 actuationTime);
	void SetRepeatTime(f32 repeatTime);

private:
	void OnKeyBackspacePressed();
	void OnKeyBackspaceDown();

	void OnAnyKeyPressed(EKey key);
	void OnAnyKeyReleased(EKey key);
	void OnAnyKeyDown(EKey key);

private:
	std::string text_;
	TextChangedFunction onTextChanged_;
	f32 actuationTime_;
	f32 repeatTime_;

	b8 isSelected_;
	UInputHoldAction holdAction_;
	char currentWriteCharacter_;
};