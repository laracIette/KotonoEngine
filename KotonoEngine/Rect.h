#pragma once
#include <kotono_framework/glm_includes.h>
#include "CoordinateSpace.h"
class URect final
{
public:
	URect();

	const glm::uvec2& GetBaseSize() const;
	const glm::vec2& GetRelativePosition() const;
	const glm::vec2& GetRelativeScale() const;
	const float GetRelativeRotation() const;

	const glm::vec2 GetWorldPosition() const;
	const glm::vec2 GetWorldScale() const;
	const float GetWorldRotation() const;

	const glm::vec2 GetScreenPosition() const;

	URect* GetParent() const;


	void SetBaseSize(const glm::uvec2& baseSize);
	void SetRelativePosition(const glm::vec2& relativePosition);
	void SetRelativeScale(const glm::vec2& relativeScale);
	void SetRelativeRotation(const float relativeRotation);

	void SetWorldPosition(const glm::vec2& worldPosition);
	void SetWorldScale(const glm::vec2& worldScale);
	void SetWorldRotation(const float worldRotation);

	void SetScreenPosition(const glm::vec2& screenPosition);
	void SetScreenSize(const glm::vec2& screenSize);

	void SetParent(URect* parent, const ECoordinateSpace keepRect);


	const glm::mat4 GetTranslationMatrix() const;
	const glm::mat4 GetRotationMatrix() const;
	const glm::mat4 GetScaleMatrix() const;
	const glm::mat4 GetModelMatrix() const;

private:
	glm::uvec2 _baseSize;
	glm::vec2 _relativePosition;
	glm::vec2 _relativeScale;
	float _relativeRotation;

	URect* _parent;
};

