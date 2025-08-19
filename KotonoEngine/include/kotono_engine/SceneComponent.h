#pragma once
#include "Object.h"
#include "Transform.h"
#include "Visibility.h"
#include "CoordinateSpace.h"
#include "Mobility.h"
#include <kotono_framework/Cached.h>

class TSceneObject;

class KSceneComponent : public KObject
{
	BASECLASS(KObject)

	friend class TSceneObject;

public:
	KSceneComponent(TSceneObject* owner);

protected:
	void Init() override;
	void Cleanup() override;

public:
	TSceneObject* GetOwner() const;
	const UTransform& GetTransform() const;
	EVisibility GetVisibility() const;
	EMobility GetMobility() const;
	bool GetCanSetTransform() const;
	KtEvent<>& GetEventTransformUpdated();

	const glm::vec3& GetRelativePosition() const;
	const glm::quat& GetRelativeRotation() const;
	const glm::vec3& GetRelativeScale() const;

	glm::vec3 GetWorldPosition() const;
	glm::quat GetWorldRotation() const;
	glm::vec3 GetWorldScale() const;

	glm::vec3 GetRightVector() const;
	glm::vec3 GetForwardVector() const;
	glm::vec3 GetUpVector() const;

	glm::mat4 GetTranslationMatrix() const;
	glm::mat4 GetRotationMatrix() const;
	glm::mat4 GetScaleMatrix() const;
	glm::mat4 GetModelMatrix();

	glm::vec3 GetScreenPosition() const;

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

	glm::vec3 GetDirection(const KSceneComponent* target) const;
	float GetDistance(const KSceneComponent* other) const;

private:
	TSceneObject* const owner_;
	KSceneComponent* parent_;
	UTransform transform_;
	EVisibility visibility_;
	EMobility mobility_;
	KtEvent<> eventTransformUpdated_;
	size_t componentIndex_;
	KtCached<glm::mat4> modelMatrix_;
};

