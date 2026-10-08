#pragma once
#include "Button.h"
#include "InputState.h"
#include <glm/ext/vector_float2.hpp>
#include <enum_utils.h>
#include <Event/Event.h>
#include <Containers/Matrix.h>
#include <types.h>

class UWindow;

class UMouse final
{
public:
	using EventMoveType = UEvent<glm::vec2, glm::vec2>;
	using EventScrollType = UEvent<glm::vec2>;
	using EventButtonType = UEvent<EButton, EInputState>;

public:
	explicit UMouse(UWindow& window);

	void Init();
	void Cleanup() const;

	void Update();

	auto GetCursorPositionDelta() const -> glm::vec2;

	void HideCursor() const;
	void ShowCursor() const;

	auto GetPreviousCursorPosition() const -> glm::vec2 { return previousCursorPosition_; }
	auto GetCursorPosition() const -> glm::vec2 { return cursorPosition_; }

	auto GetEventMove() -> EventMoveType& { return eventMove_; }
	auto GetEventScroll() -> EventScrollType& { return eventScroll_; }
	auto GetEventButton() -> EventButtonType& { return eventButton_; }

private:
	void UpdateButton(EButton button, i32 action);
	void UpdateCursorPosition(glm::vec2 const& position);
	void UpdateScrollDelta(glm::vec2 const& delta);

	template <ConvertibleTo<size> TButton, ConvertibleTo<size> TInputState>
	constexpr auto GetIsButtonState(TButton button, TInputState inputState) const noexcept -> b8
	{
		return buttonStates_[static_cast<size>(button), static_cast<size>(inputState)];
	}
	
	template <ConvertibleTo<size> TButton, ConvertibleTo<size> TInputState>
	constexpr void SetIsButtonState(TButton button, TInputState inputState, b8 value) noexcept
	{
		buttonStates_[static_cast<size>(button), static_cast<size>(inputState)] = value;
	}

private:
	UWindow& window_;

	glm::vec2 previousCursorPosition_;
	glm::vec2 cursorPosition_;
	glm::vec2 scrollDelta_;

	EventMoveType eventMove_;
	EventScrollType eventScroll_;
	EventButtonType eventButton_;

	UMatrix<b8, BUTTON_COUNT, INPUT_STATE_COUNT> buttonStates_;
};
