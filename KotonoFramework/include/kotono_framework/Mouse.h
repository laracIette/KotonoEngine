#pragma once
#include "glm_includes.h"
#include "Button.h"
#include "Event.h"
#include "InputState.h"
#include <array>
#include <unordered_set>
struct GLFWwindow;
class KtMouse final
{
	friend void mousebutton_callback_(GLFWwindow*, int, int, int);
	friend void cursorpos_callback_(GLFWwindow* window, double xpos, double ypos);
	friend void scroll_callback_(GLFWwindow*, double, double);

public:
	void Init();
	void Update();

	const glm::vec2& GetPreviousCursorPosition() const;
	const glm::vec2& GetCursorPosition() const;
	const glm::vec2 GetCursorPositionNormalized() const;
	const glm::vec2 GetCursorPositionDelta() const;
	const float GetHorizontalScrollDelta() const;
	const float GetVerticalScrollDelta() const;

	KtEvent<>& GetEvent(const KtButton button, const KtInputState inputState);
	KtEvent<glm::vec2>& GetEventMove();
	KtEvent<float>& GetEventHorizontalScroll();
	KtEvent<float>& GetEventVerticalScroll();

private:
	glm::vec2 previousCursorPosition_;
	glm::vec2 cursorPosition_;
	KtEvent<glm::vec2> eventMove_;

	float horizontalScrollDelta_;
	float verticalScrollDelta_;
	KtEvent<float> eventHorizontalScroll_;
	KtEvent<float> eventVerticalScroll_;

	std::array<std::array<KtEvent<>, KT_INPUT_STATE_COUNT>, KT_BUTTON_COUNT> buttonEvents_;
	std::array<std::unordered_set<KtInputState>, KT_BUTTON_COUNT> buttonStates_;

	void UpdateButton(const KtButton button, const int action);
};

