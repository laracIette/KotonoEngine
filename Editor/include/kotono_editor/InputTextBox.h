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
	struct InputTextBoxSettings
	{
		/// default = ""
		std::string text{ "" };
		/// default = {}
		TextChangedFunction onTextChanged{};
		/// default = 0.5f
		float actuationTime{ 0.5f };
		/// default = 0.05f
		float repeatTime{ 0.05f };
	};

	WInputTextBox(const InputTextBoxSettings& inputTextBoxSettings);

	WidgetPtr Build() override;
	void Cleanup() override;

private:
	void OnKeyBackspacePressed();
	void OnKeyBackspaceDown();

	void OnAnyKeyPressed(const EKey key);
	void OnAnyKeyReleased(const EKey key);
	void OnAnyKeyDown(const EKey key);

private:
	InputTextBoxSettings inputTextBoxSettings_;
	bool isSelected_;
	UInputHoldAction holdAction_;
	char currentWriteCharacter_;
};