#pragma once
#include "Object.h"
#include "Transform.h"
#include "Visibility.h"
#include "CoordinateSpace.h"
#include "Mobility.h"

class TSceneObject;

class KSceneComponent : public KObject
{
	BASECLASS(KObject)

	friend class TSceneObject;

public:
	KSceneComponent(TSceneObject* owner);

	void Construct() override;
	void Init() override;
	void Cleanup() override;

	TSceneObject* GetOwner() const;
	const UTransform& GetTransform() const;
	const EVisibility GetVisibility() const;
	const EMobility GetMobility() const;
	const bool GetCanSetTransform() const;
	KtEvent<>& GetEventUpdateTransform();

	const glm::vec3& GetRelativePosition() const;
	const glm::quat& GetRelativeRotation() const;
	const glm::vec3& GetRelativeScale() const;

	const glm::vec3 GetWorldPosition() const;
	const glm::quat GetWorldRotation() const;
	const glm::vec3 GetWorldScale() const;

	const glm::vec3 GetRightVector() const;
	const glm::vec3 GetForwardVector() const;
	const glm::vec3 GetUpVector() const;

	const glm::mat4 GetTranslationMatrix() const;
	const glm::mat4 GetRotationMatrix() const;
	const glm::mat4 GetScaleMatrix() const;
	const glm::mat4 GetModelMatrix() const;

	const glm::vec3 GetScreenPosition() const;

	void SetVisibility(const EVisibility visibility);
	virtual void SetMobility(const EMobility mobility);

	void SetParent(KSceneComponent* parent, const ECoordinateSpace keepTransform);

	void SetRelativePosition(const glm::vec3& relativePosition);
	void SetRelativeRotation(const glm::quat& relativeRotation);
	void SetRelativeScale(const glm::vec3& relativeScale);

	void SetWorldPosition(const glm::vec3& worldPosition);
	void SetWorldRotation(const glm::quat& worldRotation);
	void SetWorldScale(const glm::vec3& worldScale);

	void Translate(const glm::vec3& offset);
	void Rotate(const glm::quat& rotation);
	void Scale(const glm::vec3& scale);

	const glm::vec3 GetDirection(const KSceneComponent* target) const;
	const float GetDistance(const KSceneComponent* other) const;

private:
	TSceneObject* const owner_;
	KSceneComponent* parent_;
	UTransform transform_;
	EVisibility visibility_;
	EMobility mobility_;
	KtEvent<> eventUpdateTransform_;
	size_t componentIndex_;
};

