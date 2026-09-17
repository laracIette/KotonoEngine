#include "InputTextBox.h"

#include <kotono_core/Interface.h>
#include <kotono_interface/widgets.h>

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
}

void WInputTextBox::Remove()
{
	Base::Remove();
}

b8 WInputTextBox::OnKeyboardKey(EKey key, EInputState inputState)
{
	if (!isSelected_)
	{
		return INPUT_UNHANDLED;
	}

	if (key == EKey::Backspace)
	{
		switch (inputState)
		{
		case EInputState::Pressed:
		{
			holdAction_.Reset();
			return INPUT_HANDLED;
		}
		case EInputState::Down:
		{
			if (holdAction_.Update(GetInterface()->GetDeltaTime()))
			{
				SetState([this]() {
					if (!text_.empty())
					{
						text_.pop_back();
						if (onTextChanged_)
						{
							onTextChanged_(text_);
						}
					}
				});
				return INPUT_HANDLED;
			}
			break;
		}
		default:
			break;
		}
	}
	else
	{
		char const character{ keyToChar(key) };

		switch (inputState)
		{
		case EInputState::Pressed:
		{
			if (currentWriteCharacter_ != character)
			{
				currentWriteCharacter_ = character;
				holdAction_.Reset();
			}
			return INPUT_HANDLED;
		}
		case EInputState::Released:
		{
			if (currentWriteCharacter_ == character)
			{
				currentWriteCharacter_ = 0;
			}
			return INPUT_HANDLED;
		}
		case EInputState::Down:
		{
			if (currentWriteCharacter_ != character)
			{
				break;
			}

			if (!isalpha(character))
			{
				break;
			}

			if (holdAction_.Update(GetInterface()->GetDeltaTime()))
			{
				SetState([this, character]() {
					text_.push_back(character);
					if (onTextChanged_)
					{
						onTextChanged_(text_);
					}
				});
				return INPUT_HANDLED;
			}
			break;
		}
		default:
			break;
		}
	}

	return INPUT_UNHANDLED;
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

#include "generated/InputTextBox.generated.inl"
