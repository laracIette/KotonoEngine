#include "InputTextBox.h"
#include <kotono_input/Keyboard.h>
#include <kotono_interface/widgets.h>
#include <kotono_object/Interface.h>

WInputTextBox::WInputTextBox() 
	: text_{ "" }
	, onTextChanged_{}
	, actuationTime_{ 0.5f }
	, repeatTime_{ 0.05f }
	, isSelected_{ false }
	, currentWriteCharacter_{ 0 }
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

void WInputTextBox::Display(UWidgetDisplaySettings const& displaySettings)
{
	Base::Display(displaySettings);

	holdAction_.SetActuationTime(actuationTime_);
	holdAction_.SetRepeatTime(repeatTime_);

	Keyboard.GetEventAnyKey(EInputState::Pressed).AddListener(this, &WInputTextBox::OnAnyKeyPressed);
	Keyboard.GetEventAnyKey(EInputState::Released).AddListener(this, &WInputTextBox::OnAnyKeyReleased);
	Keyboard.GetEventAnyKey(EInputState::Down).AddListener(this, &WInputTextBox::OnAnyKeyDown);

	Keyboard.GetEventKey(EKey::Backspace, EInputState::Pressed).AddListener(this, &WInputTextBox::OnKeyBackspacePressed);
	Keyboard.GetEventKey(EKey::Backspace, EInputState::Down).AddListener(this, &WInputTextBox::OnKeyBackspaceDown);
}

void WInputTextBox::Remove()
{
	Base::Remove();

	Keyboard.GetEventAnyKey(EInputState::Pressed).RemoveListener(this, &WInputTextBox::OnAnyKeyPressed);
	Keyboard.GetEventAnyKey(EInputState::Released).RemoveListener(this, &WInputTextBox::OnAnyKeyReleased);
	Keyboard.GetEventAnyKey(EInputState::Down).RemoveListener(this, &WInputTextBox::OnAnyKeyDown);

	Keyboard.GetEventKey(EKey::Backspace, EInputState::Pressed).RemoveListener(this, &WInputTextBox::OnKeyBackspacePressed);
	Keyboard.GetEventKey(EKey::Backspace, EInputState::Down).RemoveListener(this, &WInputTextBox::OnKeyBackspaceDown);
}

std::string_view WInputTextBox::GetText() const
{
	return text_;
}

WInputTextBox::TextChangedFunction const& WInputTextBox::GetOnTextChanged() const
{
	return onTextChanged_;
}

f32 WInputTextBox::GetActuationTime() const
{
	return actuationTime_;
}

f32 WInputTextBox::GetRepeatTime() const
{
	return repeatTime_;
}

void WInputTextBox::SetText(std::string_view text)
{
	text_ = text;
}

void WInputTextBox::SetOnTextChanged(TextChangedFunction const& onTextChanged)
{
	onTextChanged_ = onTextChanged;
}

void WInputTextBox::SetActuationTime(f32 actuationTime)
{
	actuationTime_ = actuationTime;
}

void WInputTextBox::SetRepeatTime(f32 repeatTime)
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

	if (holdAction_.Update(GetInterface()->GetTimeContext().lastDelta))
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

void WInputTextBox::OnAnyKeyPressed(EKey key)
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

void WInputTextBox::OnAnyKeyReleased(EKey key)
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

void WInputTextBox::OnAnyKeyDown(EKey key)
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

	if (holdAction_.Update(GetInterface()->GetTimeContext().lastDelta))
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
