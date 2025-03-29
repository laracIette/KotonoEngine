#pragma once
#include <kotono_framework/glm_includes.h>
class URect final
{
public:
	URect();

	const glm::vec2& GetRelativePosition() const;
	const float GetRelativeRotation() const;
	const glm::vec2& GetRelativeScale() const;

	const glm::vec2 GetWorldPosition() const;
	const float GetWorldRotation() const;
	const glm::vec2 GetWorldScale() const;

	const glm::uvec2& GetBaseSize() const;

	URect* GetParent() const;


	void SetRelativePosition(const glm::vec2& relativePosition);
	void SetRelativeRotation(const float relativeRotation);
	void SetRelativeScale(const glm::vec2& relativeScale);

	void SetWorldPosition(const glm::vec2& worldPosition);
	void SetWorldRotation(const float worldRotation);
	void SetWorldScale(const glm::vec2& worldScale);

	void SetParent(URect* parent);

	void SetBaseSize(const glm::uvec2& baseSize);


	const glm::mat4 GetModelMatrix() const;

private:
	glm::vec2 _relativePosition;
	float _relativeRotation;
	glm::vec2 _relativeScale;

	glm::uvec2 _baseSize;

	URect* _parent;
};

