#pragma once
#include "glm_includes.h"
#include "Button.h"
#include "Event.h"
#include "InputState.h"
#include <array>
#include <unordered_set>
class KtMouse final
{
public:
	void Init();
	void Update();
	void UpdateButton(const KtButton button, const int action);
	void UpdateCursor(const glm::vec2& position);

	const glm::vec2& GetPreviousCursorPosition() const;
	const glm::vec2& GetCursorPosition() const;
	const glm::vec2 GetCursorPositionNormalized() const;
	const glm::vec2 GetCursorPositionDelta() const;
	KtEvent<>& GetButtonEvent(const KtButton button, const KtInputState inputState);
	KtEvent<>& GetMoveEvent();

private:
	glm::vec2 previousCursorPosition_;
	glm::vec2 cursorPosition_;

	std::array<std::array<KtEvent<>, KT_INPUT_STATE_COUNT>, KT_BUTTON_COUNT> buttonEvents_;
	std::array<std::unordered_set<KtInputState>, KT_BUTTON_COUNT> buttonStates_;
	KtEvent<> moveEvent_;
};

