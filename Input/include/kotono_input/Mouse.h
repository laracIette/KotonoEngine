#pragma once
#include "Button.h"
#include "InputState.h"
#include <array>
#include <glm/vec2.hpp>
#include <kotono_common/Event.h>
#include <kotono_common/types.h>
struct GLFWwindow;
class UWindow;
class GMouse final
{
	friend void mousebutton_callback_(GLFWwindow* window, i32 button, i32 action, i32 mods);
	friend void cursorpos_callback_(GLFWwindow* window, f64 xpos, f64 ypos);
	friend void scroll_callback_(GLFWwindow* window, f64 xoffset, f64 yoffset);

public:
	using EventEmptyType = UEvent<>;
	using EventMoveType = UEvent<glm::vec2, glm::vec2>;
	using EventScrollType = UEvent<glm::vec2>;
	using EventFloatType = UEvent<f32>;
	using EventAnyButtonType = UEvent<EButton, EInputState, glm::vec2>;

public:
	void Init(UWindow& window);
	void Update();

	glm::vec2 GetCursorPositionDelta() const;
	f32 GetHorizontalScrollDelta() const;
	f32 GetVerticalScrollDelta() const;

	EventEmptyType& GetEventButton(EButton button, EInputState inputState);
	b8 GetButtonState(EButton button, EInputState inputState) const;

	glm::vec2 const& GetPreviousCursorPosition() const { return previousCursorPosition_; }
	glm::vec2 const& GetCursorPosition() const { return cursorPosition_; }

	EventMoveType&		GetEventMove() { return eventMove_; }
	EventScrollType&	GetEventScroll() { return eventScroll_; }
	EventFloatType&		GetEventHorizontalScroll() { return eventHorizontalScroll_; }
	EventFloatType&		GetEventVerticalScroll() { return eventVerticalScroll_; }
	EventAnyButtonType&	GetEventAnyButton() { return eventAnyButton_; }

private:
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

	void UpdateButton(EButton button, i32 action);
};

inline GMouse Mouse;
