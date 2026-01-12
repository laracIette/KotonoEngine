#pragma once
#include "Button.h"
#include <kotono_common/Event.h>
#include "InputState.h"
#include <array>
#include <unordered_set>
#include <glm/vec2.hpp>
struct GLFWwindow;
class SMouse final
{
	friend void mousebutton_callback_(GLFWwindow* window, int button, int action, int mods);
	friend void cursorpos_callback_(GLFWwindow* window, double xpos, double ypos);
	friend void scroll_callback_(GLFWwindow* window, double xoffset, double yoffset);

public:
	void Init();
	void Update();

	const glm::vec2& PreviousCursorPosition() const;
	const glm::vec2& CursorPosition() const;
	glm::vec2 CursorPositionNormalized() const;
	glm::vec2 CursorPositionDelta() const;
	float HorizontalScrollDelta() const;
	float VerticalScrollDelta() const;

	UEvent<>& EventButton(const EButton button, const EInputState inputState);
	bool ButtonState(const EButton button, const EInputState inputState) const;
	UEvent<glm::vec2>& EventMove();
	UEvent<float>& EventHorizontalScroll();
	UEvent<float>& EventVerticalScroll();

private:
	glm::vec2 previousCursorPosition_;
	glm::vec2 cursorPosition_;
	UEvent<glm::vec2> eventMove_;

	float horizontalScrollDelta_;
	float verticalScrollDelta_;
	UEvent<float> eventHorizontalScroll_;
	UEvent<float> eventVerticalScroll_;

	std::array<std::array<UEvent<>, InputStateCount>, ButtonCount> buttonEvents_;
	std::array<std::array<bool, InputStateCount>, ButtonCount> buttonStates_;

	void UpdateButton(const EButton button, const int action);
};

inline SMouse Mouse;
