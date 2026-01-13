#pragma once
#include "ValueBox.h"
enum class Ekey : u8;
enum class EInputState : u8;
class WValueBoxString : public WValueBox<std::string>
{
	using WValueBox::WValueBox;

public:
	WWidget* Build() override;
	void Cleanup() override;

private:
	void OnKeyBackspaceDown();
	void OnKeyBackspaceReleased();

	void OnKey(const Ekey key, const EInputState inputState);

private:
	bool isSelected_;
	bool wasKeyDown_;
	bool isKeyHeld_;
	f32 keyHeldTime_;
};