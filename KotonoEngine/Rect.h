#pragma once
#include <kotono_framework/glm_includes.h>
#include "CoordinateSpace.h"
#include "Anchor.h"
#include "RotationUnit.h"

class KtViewport;

class URect final
{
public:
	URect();

	const glm::vec2& GetRelativeSize() const;
	const glm::vec2& GetRelativePosition() const;
	const glm::vec2& GetRelativeScale() const;
	const float GetRelativeRotation(const ERotationUnit unit = ERotationUnit::Radians) const;

	const glm::vec2 GetWorldSize() const;
	const glm::vec2 GetWorldPosition() const;
	const glm::vec2 GetWorldScale() const;
	const float GetWorldRotation(const ERotationUnit unit = ERotationUnit::Radians) const;

	const glm::vec2 GetScreenPosition(KtViewport* viewport) const;
	const glm::vec2 GetScreenSize(KtViewport* viewport) const;

	const EAnchor GetAnchor() const;

	URect* GetParent() const;


	void SetRelativeSize(const glm::vec2& size);
	void SetRelativePosition(const glm::vec2& relativePosition);
	void SetRelativeScale(const glm::vec2& relativeScale);
	void SetRelativeRotation(float relativeRotation, const ERotationUnit unit = ERotationUnit::Radians);

	void SetWorldSize(const glm::vec2& worldSize);
	void SetWorldPosition(const glm::vec2& worldPosition);
	void SetWorldScale(const glm::vec2& worldScale);
	void SetWorldRotation(const float worldRotation, const ERotationUnit unit = ERotationUnit::Radians);

	void AddOffset(const glm::vec2& offset);
	void AddScale(const glm::vec2& scale);
	void AddRotation(const float rotation, const ERotationUnit unit = ERotationUnit::Radians);

	void SetScreenPosition(KtViewport* viewport, const glm::vec2& screenPosition);
	void SetScreenSize(KtViewport* viewport, const glm::vec2& screenSize);

	void SetAnchor(const EAnchor anchor);

	void SetParent(URect* parent, const ECoordinateSpace keepRect);


	const float GetLeft() const;
	const float GetRight() const;
	const float GetTop() const;
	const float GetBottom() const;


	const glm::mat4 GetTranslationMatrix() const;
	const glm::mat4 GetRotationMatrix() const;
	const glm::mat4 GetScaleMatrix(KtViewport* viewport) const;
	const glm::mat4 GetModelMatrix(KtViewport* viewport) const;


	const glm::vec2 GetDirection(const URect& target) const;
	const float GetDistance(const URect& other) const;
	const bool GetIsOverlapping(const glm::vec2& worldPosition) const;
	const bool GetIsOverlapping(const URect& other) const;

private:
	glm::vec2 relativeSize_;
	glm::vec2 relativePosition_;
	glm::vec2 relativeScale_;
	float relativeRotation_;

	EAnchor anchor_;

	URect* parent_;

	const glm::vec2 GetAnchorOffset() const;
	const glm::vec2 GetWorldPositionWithAnchorOffset() const;
	const glm::vec2 GetAnchorRelativePosition() const;

	const bool GetIsOverlapping(const glm::vec2& worldPosition, const glm::vec2& worldSize) const;
};

