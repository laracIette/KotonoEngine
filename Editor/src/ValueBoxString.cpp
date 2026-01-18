#include "ValueBoxString.h"
#include <kotono_core/TimeManager.h>
#include <kotono_interface/widgets.h>
#include <kotono_input/Keyboard.h>

WWidget* WValueBoxString::Build()
{ 
	holdAction_.SetActuationTime(0.5f);
	holdAction_.SetRepeatTime(0.05f);

	Keyboard.EventKey().AddListener(UDelegate(this, &WValueBoxString::OnKey));

	Keyboard.EventKey(EKey::Backspace, EInputState::Down)
		.AddListener(UDelegate(this, &WValueBoxString::OnKeyBackspaceDown));
	Keyboard.EventKey(EKey::Backspace, EInputState::Released)
		.AddListener(UDelegate(this, &WValueBoxString::OnKeyBackspaceReleased));

	return new WWrap({
		.child = new WStack({
			.children = {
				new WColor({ UColor::White().WithAlpha(0.15f) }),
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
						.text = GetValue(),
						.fontSize = { 15.0f, 18.0f },
						.spacing = -5.0f,
					}),
				}),
			},
		}),
	});
}

void WValueBoxString::Cleanup()
{
	Keyboard.EventKey().RemoveListener(UDelegate(this, &WValueBoxString::OnKey));

	Keyboard.EventKey(EKey::Backspace, EInputState::Down)
		.RemoveListener(UDelegate(this, &WValueBoxString::OnKeyBackspaceDown));
	Keyboard.EventKey(EKey::Backspace, EInputState::Released)
		.RemoveListener(UDelegate(this, &WValueBoxString::OnKeyBackspaceReleased));

	WWidget::Cleanup();
}

void WValueBoxString::OnKeyBackspaceDown()
{
	if (!isSelected_)
	{
		return;
	}

	if (holdAction_.Update(TimeManager.Delta()))
	{
		SetState([this]()
		{
			std::string& value{ GetValue() };
			if (!value.empty())
			{
				value.pop_back();
			}
		});
	}
}

void WValueBoxString::OnKeyBackspaceReleased()
{
	if (!isSelected_)
	{
		return;
	}

	holdAction_.Reset();
}

void WValueBoxString::OnKey(const EKey key, const EInputState inputState)
{
	const char character{ keyToChar(key) };

	if (!isSelected_)
	{
		return;
	}

	if (inputState != EInputState::Down)
	{
		if (currentWriteCharacter_ == character)
		{
			holdAction_.Reset();
		}
		return;
	}

	if (!isalpha(character))
	{
		return;
	}

	if (currentWriteCharacter_ != character)
	{
		currentWriteCharacter_ = character;
	}

	if (holdAction_.Update(TimeManager.Delta()))
	{
		SetState([this, character]() 
		{
			GetValue().push_back(character);
		});
	}
}
