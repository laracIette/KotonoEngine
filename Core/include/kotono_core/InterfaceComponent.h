#pragma once
#include "generated/InterfaceComponent.generated.h"
#include "Object.h"
#include "Rect.h"
#include "CoordinateSpace.h"
#include "Anchor.h"
#include "RotationUnit.h"
#include "Visibility.h"
#include <kotono_graphics/Color.h>
#include <kotono_common/Cached.h>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>

class KtWindowViewport;
class RInterfaceObject;
struct KtInterfaceRenderableProxy;

class KInterfaceComponent : public KObject
{
	GENERATED_KINTERFACECOMPONENT()

	friend class RInterfaceObject;

public:
	KInterfaceComponent(UPtrOwnerBase* ptrOwner);

protected:
	void Init() override;
	void Cleanup() override;

public:
	const UPtr<RInterfaceObject>& GetOwner() const;
	const UPtr<KInterfaceComponent>& GetParent() const;
	const URect& GetRect() const;
	EVisibility GetVisibility() const;
	int32_t GetLayer() const;
	KtEvent<>& EventRectUpdated();
	KtEvent<>& EventLayerUpdated();
	KtEvent<>& EventColorUpdated();

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

	const KtColor& GetColor() const;

	void SetOwner(const UPtr<RInterfaceObject>& owner);
	void SetVisibility(const EVisibility visibility, const bool propagateToChildren = false);
	void SetLayer(const int32_t layer);

	void SetParent(const UPtr<KInterfaceComponent>& parent, const ECoordinateSpace keepRect);

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

	void SetColor(const KtColor& color);

	float GetLeft() const;
	float GetRight() const;
	float GetTop() const;
	float GetBottom() const;

	glm::mat4 TranslationMatrix() const;
	glm::mat4 RotationMatrix() const;
	glm::mat4 ScaleMatrix() const;
	glm::mat4 ModelMatrix();

	glm::vec2 GetDirection(const UPtr<KInterfaceComponent>& target) const;
	float GetDistance(const UPtr<KInterfaceComponent>& other) const;
	bool GetIsOverlapping(const glm::vec2& worldPosition, const glm::vec2& worldSize) const;
	bool GetIsOverlapping(const glm::vec2& worldPosition) const;
	bool GetIsOverlapping(const UPtr<KInterfaceComponent>& other) const;

	bool IsHovered() const;

	void AddChildren(const UPtr<KInterfaceComponent>& interfaceComponent);
	void RemoveChildren(const UPtr<KInterfaceComponent>& interfaceComponent);

private:
	UPtr<RInterfaceObject> owner_;
	UPtr<KInterfaceComponent> parent_;
	KtPool<UPtr<KInterfaceComponent>> children_;
	size_t childrenIndex_;
	SERIALIZE URect rect_;
	SERIALIZE EVisibility visibility_;
	SERIALIZE int32_t layer_;
	KtInterfaceRenderableProxy* boundsProxy_;
	KtEvent<> eventRectUpdated_;
	KtEvent<> eventLayerUpdated_;
	KtEvent<> eventColorUpdated_;
	size_t componentIndex_;
	KtCached<glm::mat4> modelMatrix_;
	SERIALIZE KtColor color_;

	void CreateBoundsProxy();
	void MarkBoundsProxyRectDirty();

	glm::vec2 GetAnchorOffset() const;
	glm::vec2 GetWorldPositionWithAnchorOffset() const;
	glm::vec2 GetAnchorRelativePosition() const;
};

