#pragma once
#include "ValueBox.h"
#include <kotono_input/InputHoldAction.h>
enum class EKey : u8;
enum class EInputState : u8;
class WValueBoxString : public WValueBox<std::string>
{
	using WValueBox::WValueBox;

public:
	WWidget* Build() override;
	void Cleanup() override;

private:
	void OnKeyBackspacePressed();
	void OnKeyBackspaceDown();

	void OnAnyKeyPressed(const EKey key);
	void OnAnyKeyReleased(const EKey key);
	void OnAnyKeyDown(const EKey key);

private:
	bool isSelected_;
	UInputHoldAction holdAction_;
	char currentWriteCharacter_;
};