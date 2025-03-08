#pragma once
#include <kotono_framework/glm_includes.h>
class UTransform final
{
public:
	UTransform();

	const glm::vec3& GetRelativeLocation() const;
	const glm::quat& GetRelativeRotation() const;
	const glm::vec3& GetRelativeScale() const;

	const glm::vec3 GetWorldLocation() const;
	const glm::quat GetWorldRotation() const;
	const glm::vec3 GetWorldScale() const;

	const glm::mat4 GetModelMatrix() const;

	UTransform* GetParent() const;


	void SetRelativeLocation(const glm::vec3& relativeLocation);
	void SetRelativeRotation(const glm::quat& relativeRotation);
	void SetRelativeScale(const glm::vec3& relativeScale);

	void SetWorldLocation(const glm::vec3& worldLocation);
	void SetWorldRotation(const glm::quat& worldRotation);
	void SetWorldScale(const glm::vec3& worldScale);

	void SetParent(UTransform* parent);


	const glm::vec3 GetDirection(UTransform* target) const;
	const float GetDistance(UTransform* other) const;

private:
	glm::vec3 _relativeLocation;
	glm::quat _relativeRotation;
	glm::vec3 _relativeScale;

	UTransform* _parent;
};

