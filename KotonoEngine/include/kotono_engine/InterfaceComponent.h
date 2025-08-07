#pragma once
#include "Object.h"
#include "Rect.h"
#include "CoordinateSpace.h"
#include "Anchor.h"
#include "RotationUnit.h"
#include "Visibility.h"
#include <kotono_framework/Renderable2DProxy.h>

class KtViewport;
class RInterfaceObject;

class KInterfaceComponent : public KObject
{
	BASECLASS(KObject)

	friend class RInterfaceObject;

public:
	KInterfaceComponent(RInterfaceObject* owner);

protected:
	void Init() override;
	void Cleanup() override;

public:
	RInterfaceObject* GetOwner() const;
	KInterfaceComponent* GetParent() const;
	const URect& GetRect() const;
	EVisibility GetVisibility() const;
	int32_t GetLayer() const; 
	KtEvent<>& GetEventRectUpdated();
	KtEvent<>& GetEventLayerUpdated();

	const glm::vec2& GetRelativeSize() const;
	const glm::vec2& GetRelativePosition() const;
	const glm::vec2& GetRelativeScale() const;
	float GetRelativeRotation(const ERotationUnit unit = ERotationUnit::Radians) const;

	glm::vec2 GetWorldSize() const;
	glm::vec2 GetWorldPosition() const;
	glm::vec2 GetWorldScale() const;
	float GetWorldRotation(const ERotationUnit unit = ERotationUnit::Radians) const;

	glm::vec2 GetScreenPosition() const;
	glm::vec2 GetScreenSize() const;

	EAnchor GetAnchor() const;

	void SetVisibility(const EVisibility visibility);
	void SetLayer(const int32_t layer);

	void SetParent(KInterfaceComponent* parent, const ECoordinateSpace keepRect);

	void SetRelativeSize(const glm::vec2& relativeSize);
	void SetRelativePosition(const glm::vec2& relativePosition);
	void SetRelativeScale(const glm::vec2& relativeScale);
	void SetRelativeRotation(float relativeRotation, const ERotationUnit unit = ERotationUnit::Radians);

	void SetWorldSize(const glm::vec2& worldSize);
	void SetWorldPosition(const glm::vec2& worldPosition);
	void SetWorldScale(const glm::vec2& worldScale);
	void SetWorldRotation(const float worldRotation, const ERotationUnit unit = ERotationUnit::Radians);
	
	void Translate(const glm::vec2& offset);
	void Scale(const glm::vec2& scale);
	void Rotate(const float rotation, const ERotationUnit unit = ERotationUnit::Radians);

	void SetScreenPosition(const glm::vec2& screenPosition);
	void SetScreenSize(const glm::vec2& screenSize);

	void SetAnchor(const EAnchor anchor);

	float GetLeft() const;
	float GetRight() const;
	float GetTop() const;
	float GetBottom() const;

	glm::mat4 GetTranslationMatrix() const;
	glm::mat4 GetRotationMatrix() const;
	glm::mat4 GetScaleMatrix() const;
	glm::mat4 GetModelMatrix() const;

	glm::vec2 GetDirection(const KInterfaceComponent* target) const;
	float GetDistance(const KInterfaceComponent* other) const;
	bool GetIsOverlapping(const glm::vec2& worldPosition, const glm::vec2& worldSize) const;
	bool GetIsOverlapping(const glm::vec2& worldPosition) const;
	bool GetIsOverlapping(const KInterfaceComponent* other) const;

private:
	RInterfaceObject* const owner_;
	KInterfaceComponent* parent_;
	URect rect_;
	EVisibility visibility_;
	int32_t layer_;
	KtRenderable2DProxy boundsProxy_;
	KtEvent<> eventRectUpdated_;
	KtEvent<> eventLayerUpdated_;
	size_t componentIndex_;

	void InitBoundsProxy();
	void CreateBoundsProxy();

	void MarkBoundsProxyRectDirty();

	glm::vec2 GetAnchorOffset() const;
	glm::vec2 GetWorldPositionWithAnchorOffset() const;
	glm::vec2 GetAnchorRelativePosition() const;
};

