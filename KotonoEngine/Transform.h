#pragma once
#include <kotono_framework/glm_includes.h>
class UTransform final
{
public:
	UTransform();

	const glm::vec3& GetRelativePosition() const;
	const glm::quat& GetRelativeRotation() const;
	const glm::vec3& GetRelativeScale() const;

	const glm::vec3 GetWorldPosition() const;
	const glm::quat GetWorldRotation() const;
	const glm::vec3 GetWorldScale() const;

	const glm::vec3 GetRightVector() const;
	const glm::vec3 GetForwardVector() const;
	const glm::vec3 GetUpVector() const;

	UTransform* GetParent() const;


	void SetRelativePosition(const glm::vec3& relativePosition);
	void SetRelativeRotation(const glm::quat& relativeRotation);
	void SetRelativeScale(const glm::vec3& relativeScale);

	void SetWorldPosition(const glm::vec3& worldPosition);
	void SetWorldRotation(const glm::quat& worldRotation);
	void SetWorldScale(const glm::vec3& worldScale);

	void AddPosition(const glm::vec3& position);
	void AddRotation(const glm::quat& rotation);
	void AddScale(const glm::vec3& scale);

	void SetParent(UTransform* parent);


	const glm::mat4 GetTranslationMatrix() const;
	const glm::mat4 GetRotationMatrix() const;
	const glm::mat4 GetScaleMatrix() const;
	const glm::mat4 GetModelMatrix() const;


	const glm::vec3 GetDirection(UTransform* target) const;
	const float GetDistance(UTransform* other) const;

private:
	glm::vec3 _relativePosition;
	glm::quat _relativeRotation;
	glm::vec3 _relativeScale;

	UTransform* _parent;
};

