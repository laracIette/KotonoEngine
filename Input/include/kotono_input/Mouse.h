#pragma once
#include "Button.h"
#include "InputState.h"
#include <array>
#include <glm/ext/vector_float2.hpp>
#include <kotono_common/Event.h>
#include <kotono_common/types.h>
struct GLFWwindow;
class UWindow;
class UMouse final
{
public:
	using EventMoveType = UEvent<glm::vec2, glm::vec2>;
	using EventScrollType = UEvent<glm::vec2>;
	using EventButtonType = UEvent<EButton, EInputState, glm::vec2>;

public:
	UMouse(UWindow& window);

	void Init();
	void Cleanup();

	void Update();

	auto GetCursorPositionDelta() const -> glm::vec2;

	auto GetButtonState(EButton button, EInputState inputState) const -> b8;

	void HideCursor() const;
	void ShowCursor() const;

	auto GetPreviousCursorPosition() const -> glm::vec2 { return previousCursorPosition_; }
	auto GetCursorPosition() const -> glm::vec2 { return cursorPosition_; }

	auto GetEventMove() -> EventMoveType& { return eventMove_; }
	auto GetEventScroll() -> EventScrollType& { return eventScroll_; }
	auto GetEventButton() -> EventButtonType& { return eventButton_; }

private:
	void UpdateButton(GLFWwindow* window, EButton button, i32 action);
	void UpdateCursorPosition(GLFWwindow* window, glm::vec2 const& position);
	void UpdateScrollDelta(GLFWwindow* window, glm::vec2 const& delta);

private:
	UWindow& window_;

	glm::vec2 previousCursorPosition_;
	glm::vec2 cursorPosition_;
	glm::vec2 scrollDelta_;

	EventMoveType eventMove_;
	EventScrollType eventScroll_;
	EventButtonType eventButton_;

	std::array<std::array<b8, InputStateCount>, ButtonCount> buttonStates_;
};
