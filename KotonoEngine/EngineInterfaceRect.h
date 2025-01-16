#pragma once
#include <glm/glm.hpp>
#include "Edge.h"
class EngineInterfaceRect
{
public:
	const glm::vec2& GetPosition() const;
	const glm::vec2& GetSize() const;

	void SetPosition(const glm::vec2& position);
	void SetSize(const glm::vec2& size);

	const bool Overlaps(const glm::vec2& position) const;

	// expects normalized position
	const Edge GetClosestEdge(const glm::vec2& position) const;

private:
	glm::vec2 _position;
	glm::vec2 _size;
};

