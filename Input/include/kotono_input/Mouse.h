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
	using EventEmptyType = UEvent<>;
	using EventMoveType = UEvent<glm::vec2, glm::vec2>;
	using EventScrollType = UEvent<glm::vec2, glm::vec2>;
	using EventFloatType = UEvent<f32>;
	using EventAnyButtonType = UEvent<EButton, EInputState, glm::vec2>;

public:
	UMouse(UWindow& window);

	void Init();
	void Cleanup();

	void Update();

	glm::vec2 GetCursorPositionDelta() const;
	f32 GetHorizontalScrollDelta() const;
	f32 GetVerticalScrollDelta() const;

	EventEmptyType& GetEventButton(EButton button, EInputState inputState);
	b8 GetButtonState(EButton button, EInputState inputState) const;

	void HideCursor() const;
	void ShowCursor() const;

	glm::vec2 const& GetPreviousCursorPosition() const { return previousCursorPosition_; }
	glm::vec2 const& GetCursorPosition() const { return cursorPosition_; }

	EventMoveType&		GetEventMove() { return eventMove_; }
	EventScrollType&	GetEventScroll() { return eventScroll_; }
	EventFloatType&		GetEventHorizontalScroll() { return eventHorizontalScroll_; }
	EventFloatType&		GetEventVerticalScroll() { return eventVerticalScroll_; }
	EventAnyButtonType&	GetEventAnyButton() { return eventAnyButton_; }

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
	EventFloatType eventHorizontalScroll_;
	EventFloatType eventVerticalScroll_;

	EventAnyButtonType eventAnyButton_;

	std::array<std::array<EventEmptyType, InputStateCount>, ButtonCount> buttonEvents_;
	std::array<std::array<b8, InputStateCount>, ButtonCount> buttonStates_;
};
