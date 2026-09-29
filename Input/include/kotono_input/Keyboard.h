#pragma once
#include "InputState.h"
#include "Key.h"
#include "Modifier.h"
#include <kotono_common/enum_utils.h>
#include <kotono_common/Event.h>
#include <kotono_common/Matrix.h>
#include <kotono_common/types.h>

struct GLFWwindow;
class UWindow;

class UKeyboard final
{
public:	
	using EventKeyType = UEvent<EKey, EInputState>;

public:
	UKeyboard(UWindow& window);

	void Init();
	void Cleanup();

	void Update();

	auto GetEventKey() -> EventKeyType& { return eventKey_; }
	auto GetModifier() const -> EModifier { return modifier_; }

private:
	void UpdateKey(GLFWwindow* window, EKey key, i32 action);
	
	template <ConvertibleTo<size> TKey, ConvertibleTo<size> TInputState>
	constexpr auto GetIsKeyState(TKey key, TInputState inputState) const noexcept -> b8
	{
		return keyStates_[static_cast<size>(key), static_cast<size>(inputState)];
	}
	
	template <ConvertibleTo<size> TKey, ConvertibleTo<size> TInputState>
	constexpr void SetIsKeyState(TKey key, TInputState inputState, b8 value) noexcept
	{
		keyStates_[static_cast<size>(key), static_cast<size>(inputState)] = value;
	}

private:
	UWindow& window_;

	EventKeyType eventKey_;
	EModifier modifier_;

	UMatrix<b8, KeyCount, InputStateCount> keyStates_;
};
