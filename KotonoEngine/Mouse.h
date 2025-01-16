#pragma once
#include <glm/glm.hpp>
class Mouse final
{
public:
	const glm::vec2& GetCursorPosition() const;
	const glm::vec2 GetCursorPositionNormalized() const;

	void SetCursorPosition(const glm::vec2& position);

private:
	glm::vec2 _cursorPosition;
};

