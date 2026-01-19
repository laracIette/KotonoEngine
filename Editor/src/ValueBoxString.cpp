#include "ValueBoxString.h"
#include <kotono_core/TimeManager.h>
#include <kotono_interface/widgets.h>
#include <kotono_input/Keyboard.h>

WWidget* WValueBoxString::Build()
{ 
	holdAction_.SetActuationTime(0.5f);
	holdAction_.SetRepeatTime(0.05f);

	Keyboard.EventAnyKey(EInputState::Down)
		.AddListener(UDelegate(this, &WValueBoxString::OnAnyKeyDown));
	Keyboard.EventAnyKey(EInputState::Released)
		.AddListener(UDelegate(this, &WValueBoxString::OnAnyKeyReleased));

	Keyboard.EventKey(EKey::Backspace, EInputState::Down)
		.AddListener(UDelegate(this, &WValueBoxString::OnKeyBackspaceDown));
	Keyboard.EventKey(EKey::Backspace, EInputState::Released)
		.AddListener(UDelegate(this, &WValueBoxString::OnKeyBackspaceReleased));

	return new WWrap({
		.axis = WWrap::Axis::Vertical,
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
	Keyboard.EventAnyKey(EInputState::Down)
		.RemoveListener(UDelegate(this, &WValueBoxString::OnAnyKeyDown));
	Keyboard.EventAnyKey(EInputState::Released)
		.RemoveListener(UDelegate(this, &WValueBoxString::OnAnyKeyReleased));

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

void WValueBoxString::OnAnyKeyDown(const EKey key)
{
	const char character{ keyToChar(key) };

	if (!isSelected_)
	{
		return;
	}

	if (!isalpha(character))
	{
		return;
	}

	if (currentWriteCharacter_ != character)
	{
		currentWriteCharacter_ = character;
		holdAction_.Reset();
	}

	if (holdAction_.Update(TimeManager.Delta()))
	{
		SetState([this, character]() 
		{
			GetValue().push_back(character);
		});
	}
}

void WValueBoxString::OnAnyKeyReleased(const EKey key)
{
	const char character{ keyToChar(key) };

	if (currentWriteCharacter_ != character)
	{
		return;
	}

	holdAction_.Reset();
}
