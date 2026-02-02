#include "InputTextBox.h"
#include <kotono_core/TimeManager.h>
#include <kotono_input/Keyboard.h>
#include <kotono_interface/widgets.h>

WInputTextBox::WInputTextBox(const InputTextBoxSettings& inputTextBoxSettings) 
	: inputTextBoxSettings_(inputTextBoxSettings)
	, isSelected_(false)
	, currentWriteCharacter_(0)
{
}

WWidget* WInputTextBox::Build()
{
	holdAction_.SetActuationTime(inputTextBoxSettings_.actuationTime);
	holdAction_.SetRepeatTime(inputTextBoxSettings_.repeatTime);

	Keyboard.EventAnyKey(EInputState::Pressed)
		.AddListener(UDelegate(this, &WInputTextBox::OnAnyKeyPressed));
	Keyboard.EventAnyKey(EInputState::Released)
		.AddListener(UDelegate(this, &WInputTextBox::OnAnyKeyReleased));
	Keyboard.EventAnyKey(EInputState::Down)
		.AddListener(UDelegate(this, &WInputTextBox::OnAnyKeyDown));

	Keyboard.EventKey(EKey::Backspace, EInputState::Pressed)
		.AddListener(UDelegate(this, &WInputTextBox::OnKeyBackspacePressed));
	Keyboard.EventKey(EKey::Backspace, EInputState::Down)
		.AddListener(UDelegate(this, &WInputTextBox::OnKeyBackspaceDown));

	return new WStack({
		.children = {
			isSelected_ 
				? new WColor({ UColor::White().WithAlpha(0.15f) })
				: new WColor({ UColor::White().WithAlpha(0.05f) }),
			new WButton({
				.onPress = [this]() {
					SetState([this]() { isSelected_ = true; });
				},
				.onPressOut = [this]() {
					SetState([this]() { isSelected_ = false; });
				},
			}),
			new WPadding({
				.padding = WPadding::Padding::All(4.0f),
				.child = new WText({
					.text = inputTextBoxSettings_.text,
					.fontSize = { 15.0f, 18.0f },
					.spacing = -5.0f,
				}),
			}),
		},
	});
}

void WInputTextBox::Cleanup()
{
	Keyboard.EventAnyKey(EInputState::Pressed)
		.RemoveListener(UDelegate(this, &WInputTextBox::OnAnyKeyPressed));
	Keyboard.EventAnyKey(EInputState::Released)
		.RemoveListener(UDelegate(this, &WInputTextBox::OnAnyKeyReleased));
	Keyboard.EventAnyKey(EInputState::Down)
		.RemoveListener(UDelegate(this, &WInputTextBox::OnAnyKeyDown));

	Keyboard.EventKey(EKey::Backspace, EInputState::Pressed)
		.RemoveListener(UDelegate(this, &WInputTextBox::OnKeyBackspacePressed));
	Keyboard.EventKey(EKey::Backspace, EInputState::Down)
		.RemoveListener(UDelegate(this, &WInputTextBox::OnKeyBackspaceDown));

	WWidget::Cleanup();
}

void WInputTextBox::OnKeyBackspacePressed()
{
	if (!isSelected_)
	{
		return;
	}

	holdAction_.Reset();
}

void WInputTextBox::OnKeyBackspaceDown()
{
	if (!isSelected_)
	{
		return;
	}

	if (holdAction_.Update(TimeManager.Delta()))
	{
		SetState([this]()
		{
			if (!inputTextBoxSettings_.text.empty())
			{
				inputTextBoxSettings_.text.pop_back();
				if (inputTextBoxSettings_.onTextChanged)
				{
					inputTextBoxSettings_.onTextChanged(inputTextBoxSettings_.text);
				}
			}
		});
	}
}

void WInputTextBox::OnAnyKeyPressed(const EKey key)
{
	if (!isSelected_)
	{
		return;
	}

	const char character{ keyToChar(key) };

	if (currentWriteCharacter_ != character)
	{
		currentWriteCharacter_ = character;
		holdAction_.Reset();
	}
}

void WInputTextBox::OnAnyKeyReleased(const EKey key)
{
	if (!isSelected_)
	{
		return;
	}

	const char character{ keyToChar(key) };

	if (currentWriteCharacter_ == character)
	{
		currentWriteCharacter_ = 0;
	}
}

void WInputTextBox::OnAnyKeyDown(const EKey key)
{
	const char character{ keyToChar(key) };

	if (!isSelected_)
	{
		return;
	}

	if (currentWriteCharacter_ != character)
	{
		return;
	}

	if (!isalpha(character))
	{
		return;
	}

	if (holdAction_.Update(TimeManager.Delta()))
	{
		SetState([this, character]()
		{
			inputTextBoxSettings_.text.push_back(character);
			if (inputTextBoxSettings_.onTextChanged)
			{
				inputTextBoxSettings_.onTextChanged(inputTextBoxSettings_.text);
			}
		});
	}
}
