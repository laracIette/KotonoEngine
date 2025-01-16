#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class NDCRect
{
public:
	NDCRect(const glm::vec2& position, const glm::quat& rotation, const glm::vec2& size);

	const glm::mat4 GetModelMatrix() const;

private:
	const glm::vec2 _position;
	const glm::quat _rotation;
	const glm::vec2 _size;
};

