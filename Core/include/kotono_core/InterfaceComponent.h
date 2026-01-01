#pragma once
#include "generated/InterfaceComponent.generated.h"
#include "Anchor.h"
#include "CoordinateSpace.h"
#include "Object.h"
#include "Rect.h"
#include "RotationUnit.h"
#include "Visibility.h"
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <kotono_common/Cached.h>
#include <kotono_common/Event.h>
#include <kotono_graphics/Color.h>

class KtWindowViewport;
class RInterfaceObject;
class UInterfaceProxy;

class KInterfaceComponent : public KObject
{
	GENERATED_KINTERFACECOMPONENT()

private:
	friend class RInterfaceObject;

public:
	KInterfaceComponent(UPtrOwnerBase* ptrOwner);

protected:
	void Cleanup() override;

	virtual void Init();
	virtual void Update(const float deltaTime);

public:
	const UPtr<RInterfaceObject>& GetOwner() const;
	const UPtr<KInterfaceComponent>& GetParent() const;
	bool GetCanUpdate() const;
	const URect& GetRect() const;
	EVisibility GetVisibility() const;
	int32_t GetLayer() const;
	KtEvent<>& EventRectChanged();
	KtEvent<>& EventLayerChanged();
	KtEvent<>& EventColorChanged();

	const glm::vec2& GetRelativePosition() const;
	const glm::vec2& GetRelativeScale() const;
	float GetRelativeRotation(const ERotationUnit unit = ERotationUnit::Radians) const;

	glm::vec2 GetWorldPosition() const;
	glm::vec2 GetWorldScale() const;
	float GetWorldRotation(const ERotationUnit unit = ERotationUnit::Radians) const;

	EAnchor GetAnchor() const;

	const UColor& GetColor() const;

	void SetOwner(const UPtr<RInterfaceObject>& owner);
	void SetCanUpdate(const bool canUpdate);
	void SetVisibility(const EVisibility visibility, const bool propagateToChildren = false);
	void SetLayer(const int32_t layer);

	void SetParent(const UPtr<KInterfaceComponent>& parent, const ECoordinateSpace keepRect);

	void SetRelativePosition(const glm::vec2& relativePosition);
	void SetRelativeScale(const glm::vec2& relativeScale);
	void SetRelativeRotation(float relativeRotation, const ERotationUnit unit = ERotationUnit::Radians);

	void SetWorldPosition(const glm::vec2& worldPosition);
	void SetWorldScale(const glm::vec2& worldScale);
	void SetWorldRotation(const float worldRotation, const ERotationUnit unit = ERotationUnit::Radians);

	void Translate(const glm::vec2& offset);
	void Scale(const glm::vec2& scale);
	void Rotate(const float rotation, const ERotationUnit unit = ERotationUnit::Radians);

	void SetAnchor(const EAnchor anchor);

	void SetColor(const UColor& color);

	glm::mat4 TranslationMatrix() const;
	glm::mat4 RotationMatrix() const;
	glm::mat4 ScaleMatrix() const;
	glm::mat4 ModelMatrix();

	glm::vec2 GetDirection(const UPtr<KInterfaceComponent>& target) const;
	float GetDistance(const UPtr<KInterfaceComponent>& other) const;
	bool GetIsOverlapping(const glm::vec2& worldPosition, const glm::vec2& worldScale) const;
	bool GetIsOverlapping(const glm::vec2& worldPosition) const;
	bool GetIsOverlapping(const UPtr<KInterfaceComponent>& other) const;

	bool IsHovered() const;

	void AddChildren(const UPtr<KInterfaceComponent>& interfaceComponent);
	void RemoveChildren(const UPtr<KInterfaceComponent>& interfaceComponent);

	virtual void Spawn();

private:
	//void CreateBoundsProxy();
	//void MarkBoundsProxyRectDirty();

	glm::vec2 GetAnchorOffset() const;
	glm::vec2 GetWorldPositionWithAnchorOffset() const;
	glm::vec2 GetAnchorRelativePosition() const;

private:
	bool isInit_;
	bool canUpdate_;
	UPtr<RInterfaceObject> owner_;
	UPtr<KInterfaceComponent> parent_;
	KtPool<UPtr<KInterfaceComponent>> children_;
	size_t childrenIndex_;
	SERIALIZE URect rect_;
	SERIALIZE EVisibility visibility_;
	SERIALIZE UColor color_;
	//UInterfaceProxy* boundsProxy_;
	KtEvent<> eventRectChanged_;
	KtEvent<> eventLayerChanged_;
	KtEvent<> eventColorChanged_;
	KtCached<glm::mat4> modelMatrix_;
	size_t componentIndex_;
};

