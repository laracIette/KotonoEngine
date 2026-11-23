#include "InputTextBox.h"

#include <Interface/Interface.h>
#include <core_widgets.h>

WInputTextBox::WInputTextBox()
	: text_{}
    , isEditable_{ false }
	, onTextChanged_{}
	, actuationTime_{ 0.5f }
	, repeatTime_{ 0.05f }
	, currentWriteCharacter_{ 0 }
{
}

auto WInputTextBox::Build() -> WidgetPtr
{
	return (
		UCreate<WStack>{}()
		| (
			UCreate<WColor>{}()
			| Apply(&WColor::SetColor, [this]() { return GetIsFocused() ? Colors::White.WithAlpha(0.15f) : Colors::White.WithAlpha(0.05f); })
		)
		| (
			UCreate<WPadding>{}()
			| Apply(&WPadding::SetPadding, UPadding::All(4.0f))
			| (
				UCreate<WText>{}()
				| Apply(&WText::SetText, [this]() { return valueToString_ ? valueToString_() : ""; })
			)
		)
	);
}

auto WInputTextBox::OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) -> b8
{
    if (!isEditable_)
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

void WInputTextBox::SetActuationTime(f32 actuationTime)
{
	actuationTime_ = actuationTime;
	holdAction_.SetActuationTime(actuationTime);
}

void WInputTextBox::SetRepeatTime(f32 repeatTime)
{
	repeatTime_ = repeatTime;
	holdAction_.SetRepeatTime(repeatTime);
}

#include "InputTextBox.generated.inl"
