#include "NDCRect.h"
#include "KotonoEngine.h"

NDCRect::NDCRect(const glm::vec2& position, const glm::quat& rotation, const glm::vec2& size) :
	_position(position),
	_rotation(rotation),
	_size(size)
{
}

const glm::mat4 NDCRect::GetModelMatrix() const
{
    glm::vec2 windowSize = Engine.GetWindow().GetSize();

    glm::vec3 ndcPosition(
        2.0f * _position.x / windowSize.x - 1.0f,
        1.0f - _position.y / windowSize.y * 2.0f,
        0.0f
    );

    glm::vec3 ndcSize(_size / windowSize, 1.0f);

    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), ndcSize);
    glm::mat4 rotationMatrix = glm::mat4_cast(_rotation);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), ndcPosition);

    return translationMatrix * rotationMatrix * scaleMatrix;
}
