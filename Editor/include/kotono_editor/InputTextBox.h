#include <kotono_interface/Widget.h>
#include <kotono_input/InputHoldAction.h>
enum class EKey : u8;
enum class EInputState : u8;
class WInputTextBox : public WWidget
{
public:
	WInputTextBox(const std::string& text);

	WWidget* Build() override;
	void Cleanup() override;

private:
	void OnKeyBackspacePressed();
	void OnKeyBackspaceDown();

	void OnAnyKeyPressed(const EKey key);
	void OnAnyKeyReleased(const EKey key);
	void OnAnyKeyDown(const EKey key);

private:
	std::string text_;
	bool isSelected_;
	UInputHoldAction holdAction_;
	char currentWriteCharacter_;
};