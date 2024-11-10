#pragma once
#include <glm/gtc/quaternion.hpp>
#include "Anchor.h"
class Rect final
{
public:
	Rect();

	// Getters
	const glm::vec2& GetBaseSize() const;

	const Anchor GetAnchor() const;
	
	const glm::vec2 GetRelativePosition() const;
	const glm::quat& GetRelativeRotation() const;
	const glm::vec2& GetRelativeScale() const;
	const glm::vec2 GetRelativeSize() const;

	const glm::vec2 GetWorldPosition() const;
	const glm::quat GetWorldRotation() const;
	const glm::vec2 GetWorldScale() const;
	const glm::vec2 GetWorldSize() const;

	const glm::vec2& GetRelativePositionVelocity() const;

	const glm::mat4 GetModelMatrix() const;

	Rect* GetParent() const;

	// Setters
	void SetBaseSize(const glm::vec2& baseSize);

	void SetAnchor(const Anchor anchor);
	
	void SetRelativePosition(const glm::vec2& relativePosition);
	void SetRelativeRotation(const glm::quat& relativeRotation);
	void SetRelativeScale(const glm::vec2& relativeScale);
	void SetRelativeSize(const glm::vec2& relativeSize);

	void SetWorldPosition(const glm::vec2& worldPosition);
	void SetWorldRotation(const glm::quat& worldRotation);
	void SetWorldScale(const glm::vec2& worldScale);
	void SetWorldSize(const glm::vec2& worldSize);

	void SetRelativePositionVelocity(const glm::vec2& relativePositionVelocity);

	void SetParent(Rect* parent);

	// Functions
	void Update(float deltaTime);

	const glm::vec2 GetDirection(Rect* target) const;
	const float GetDistance(Rect* other) const;

private:
	glm::vec2 _baseSize;

	Anchor _anchor;

	glm::vec2 _relativePosition;
	glm::quat _relativeRotation;
	glm::vec2 _relativeScale;

	glm::vec2 _relativePositionVelocity;

	Rect* _parent;

	const glm::vec2 GetAnchorPositionDelta() const;
};

