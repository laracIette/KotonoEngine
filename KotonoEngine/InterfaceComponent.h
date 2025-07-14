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

	void Init() override;
	void Cleanup() override;

	RInterfaceObject* GetOwner() const;
	KInterfaceComponent* GetParent() const;
	const URect& GetRect() const;
	const EVisibility GetVisibility() const;
	const int32_t GetLayer() const; 
	KtEvent<>& GetEventRectUpdated();
	KtEvent<>& GetEventLayerUpdated();

	const glm::vec2& GetRelativeSize() const;
	const glm::vec2& GetRelativePosition() const;
	const glm::vec2& GetRelativeScale() const;
	const float GetRelativeRotation(const ERotationUnit unit = ERotationUnit::Radians) const;

	const glm::vec2 GetWorldSize() const;
	const glm::vec2 GetWorldPosition() const;
	const glm::vec2 GetWorldScale() const;
	const float GetWorldRotation(const ERotationUnit unit = ERotationUnit::Radians) const;

	const glm::vec2 GetScreenPosition() const;
	const glm::vec2 GetScreenSize() const;

	const EAnchor GetAnchor() const;

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

	const float GetLeft() const;
	const float GetRight() const;
	const float GetTop() const;
	const float GetBottom() const;

	const glm::mat4 GetTranslationMatrix() const;
	const glm::mat4 GetRotationMatrix() const;
	const glm::mat4 GetScaleMatrix() const;
	const glm::mat4 GetModelMatrix() const;

	const glm::vec2 GetDirection(const KInterfaceComponent* target) const;
	const float GetDistance(const KInterfaceComponent* other) const;
	const bool GetIsOverlapping(const glm::vec2& worldPosition, const glm::vec2& worldSize) const;
	const bool GetIsOverlapping(const glm::vec2& worldPosition) const;
	const bool GetIsOverlapping(const KInterfaceComponent* other) const;

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

	const glm::vec2 GetAnchorOffset() const;
	const glm::vec2 GetWorldPositionWithAnchorOffset() const;
	const glm::vec2 GetAnchorRelativePosition() const;
};

