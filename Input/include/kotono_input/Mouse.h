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

	KtEvent<>& EventButton(const KtButton button, const KtInputState inputState);
	bool ButtonState(const KtButton button, const KtInputState inputState) const;
	KtEvent<glm::vec2>& EventMove();
	KtEvent<float>& EventHorizontalScroll();
	KtEvent<float>& EventVerticalScroll();

private:
	glm::vec2 previousCursorPosition_;
	glm::vec2 cursorPosition_;
	KtEvent<glm::vec2> eventMove_;

	float horizontalScrollDelta_;
	float verticalScrollDelta_;
	KtEvent<float> eventHorizontalScroll_;
	KtEvent<float> eventVerticalScroll_;

	std::array<std::array<KtEvent<>, KT_INPUT_STATE_COUNT>, KT_BUTTON_COUNT> buttonEvents_;
	std::array<std::array<bool, KT_INPUT_STATE_COUNT>, KT_BUTTON_COUNT> buttonStates_;

	void UpdateButton(const KtButton button, const int action);
};

inline SMouse Mouse;
