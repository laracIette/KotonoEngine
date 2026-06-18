#pragma once
#include "generated/InputTextBox.generated.h"
#include <kotono_interface/Widget.h>
#include <kotono_input/InputHoldAction.h>
enum class EKey : u8;
enum class EInputState : u8;
class WInputTextBox : public WWidget
{
	GENERATED_WINPUTTEXTBOX()

	using TextChangedFunction = std::function<void(const std::string&)>;

public:
	WInputTextBox();

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

public:
	const std::string& GetText() const;
	const TextChangedFunction& GetOnTextChanged() const;
	float GetActuationTime() const;
	float GetRepeatTime() const;

	void SetText(const std::string& text);
	void SetOnTextChanged(const TextChangedFunction& onTextChanged);
	void SetActuationTime(const float actuationTime);
	void SetRepeatTime(const float repeatTime);

private:
	void OnKeyBackspacePressed();
	void OnKeyBackspaceDown();

	void OnAnyKeyPressed(const EKey key);
	void OnAnyKeyReleased(const EKey key);
	void OnAnyKeyDown(const EKey key);

private:
	std::string text_;
	TextChangedFunction onTextChanged_;
	float actuationTime_;
	float repeatTime_;

	bool isSelected_;
	UInputHoldAction holdAction_;
	char currentWriteCharacter_;
};