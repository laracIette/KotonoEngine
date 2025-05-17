#pragma once
#include "glm_includes.h"
#include "Button.h"
#include "Event.h"
#include "InputState.h"
#include <map>
#include <unordered_set>
class KtMouse final
{
public:
	void Update();

	const glm::vec2& GetPreviousCursorPosition() const;
	const glm::vec2& GetCursorPosition() const;
	const glm::vec2 GetCursorPositionNormalized() const;
	const glm::vec2 GetCursorPositionDelta() const;
	KtEvent<>& GetButtonEvent(const KtButton button, const KtInputState inputState);
	KtEvent<>& GetMoveEvent();

private:
	glm::vec2 previousCursorPosition_;
	glm::vec2 cursorPosition_;

	std::map<KtButton, std::map<KtInputState, KtEvent<>>> buttonEvents_;
	std::map<KtButton, std::unordered_set<KtInputState>> buttonStates_;
	KtEvent<> moveEvent_;
};

