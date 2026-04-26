#include "InputTextBox.h"
#include <kotono_core/TimeManager.h>
#include <kotono_input/Keyboard.h>
#include <kotono_interface/widgets.h>

WInputTextBox::WInputTextBox() 
	: text_("")
	, onTextChanged_({})
	, actuationTime_(0.5f)
	, repeatTime_(0.05f)
	, isSelected_(false)
	, currentWriteCharacter_(0)
{
}

WidgetPtr WInputTextBox::Build()
{
	UPtr text{ UCreate<WText>{}() };
	text->SetText(text_);
	text->SetFontSize({ 15.0f, 18.0f });
	text->SetSpacing(-5.0f);

	UPtr textPadding{ UCreate<WPadding>{}() };
	textPadding->SetChild(text);
	textPadding->SetPadding(UPadding::All(4.0f));


	UPtr button{ UCreate<WButton>{}() };
	button->SetOnPressed([this]() {
		SetState([this]() { isSelected_ = true; });
	});
	button->SetOnPressOut([this]() {
		SetState([this]() { isSelected_ = true; });
	});

	
	UPtr bg{ UCreate<WColor>{}() };
	bg->SetColor(isSelected_
		? Colors::White.WithAlpha(0.15f)
		: Colors::White.WithAlpha(0.05f)
	);


	UPtr stack{ UCreate<WStack>{}() };
	stack->SetChildren({ bg, button, textPadding });

	return stack;
}

void WInputTextBox::Display(UWidgetDisplaySettings displaySettings)
{
	Base::Display(displaySettings);

	holdAction_.SetActuationTime(actuationTime_);
	holdAction_.SetRepeatTime(repeatTime_);

	Keyboard.EventAnyKey(EInputState::Pressed).AddListener(this, &WInputTextBox::OnAnyKeyPressed);
	Keyboard.EventAnyKey(EInputState::Released).AddListener(this, &WInputTextBox::OnAnyKeyReleased);
	Keyboard.EventAnyKey(EInputState::Down).AddListener(this, &WInputTextBox::OnAnyKeyDown);

	Keyboard.EventKey(EKey::Backspace, EInputState::Pressed).AddListener(this, &WInputTextBox::OnKeyBackspacePressed);
	Keyboard.EventKey(EKey::Backspace, EInputState::Down).AddListener(this, &WInputTextBox::OnKeyBackspaceDown);
}

void WInputTextBox::Remove()
{
	Base::Remove();

	Keyboard.EventAnyKey(EInputState::Pressed).RemoveListener(this, &WInputTextBox::OnAnyKeyPressed);
	Keyboard.EventAnyKey(EInputState::Released).RemoveListener(this, &WInputTextBox::OnAnyKeyReleased);
	Keyboard.EventAnyKey(EInputState::Down).RemoveListener(this, &WInputTextBox::OnAnyKeyDown);

	Keyboard.EventKey(EKey::Backspace, EInputState::Pressed).RemoveListener(this, &WInputTextBox::OnKeyBackspacePressed);
	Keyboard.EventKey(EKey::Backspace, EInputState::Down).RemoveListener(this, &WInputTextBox::OnKeyBackspaceDown);
}

const std::string& WInputTextBox::GetText() const
{
	return text_;
}

const WInputTextBox::TextChangedFunction& WInputTextBox::GetOnTextChanged() const
{
	return onTextChanged_;
}

float WInputTextBox::GetActuationTime() const
{
	return actuationTime_;
}

float WInputTextBox::GetRepeatTime() const
{
	return repeatTime_;
}

void WInputTextBox::SetText(const std::string& text)
{
	text_ = text;
}

void WInputTextBox::SetOnTextChanged(const TextChangedFunction& onTextChanged)
{
	onTextChanged_ = onTextChanged;
}

void WInputTextBox::SetActuationTime(const float actuationTime)
{
	actuationTime_ = actuationTime;
}

void WInputTextBox::SetRepeatTime(const float repeatTime)
{
	repeatTime_ = repeatTime;
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
			if (!text_.empty())
			{
				text_.pop_back();
				if (onTextChanged_)
				{
					onTextChanged_(text_);
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
			text_.push_back(character);
			if (onTextChanged_)
			{
				onTextChanged_(text_);
			}
		});
	}
}

#include "generated/InputTextBox.generated.inl"
