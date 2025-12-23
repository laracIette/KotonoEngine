#pragma once
#include "generated/SceneComponent.generated.h"
#include "Object.h"
#include "Transform.h"
#include "Visibility.h"
#include "CoordinateSpace.h"
#include "Mobility.h"
#include <kotono_common/Cached.h>
#include <kotono_common/Pool.h>

class TSceneObject;

class KSceneComponent : public KObject
{
	GENERATED_KSCENECOMPONENT()

	friend class TSceneObject;

public:
	KSceneComponent(UPtrOwnerBase* ptrOwner);

protected:
	void Init() override;
	void Cleanup() override;

	virtual void Update();

public:
	const UPtr<TSceneObject>& GetOwner() const;
	bool GetCanUpdate() const;
	const UTransform& GetTransform() const;
	EVisibility GetVisibility() const;
	EMobility GetMobility() const;
	bool CanSetTransform() const;
	KtEvent<>& EventTransformUpdated();

	const glm::vec3& GetRelativePosition() const;
	const glm::quat& GetRelativeRotation() const;
	const glm::vec3& GetRelativeScale() const;

	glm::vec3 GetWorldPosition() const;
	glm::quat GetWorldRotation() const;
	glm::vec3 GetWorldScale() const;

	glm::vec3 RightVector() const;
	glm::vec3 ForwardVector() const;
	glm::vec3 UpVector() const;

	glm::mat4 TranslationMatrix() const;
	glm::mat4 RotationMatrix() const;
	glm::mat4 ScaleMatrix() const;
	const glm::mat4& ModelMatrix();

	glm::vec3 GetScreenPosition() const;

	void SetOwner(const UPtr<TSceneObject>& owner);
	void SetCanUpdate(const bool canUpdate);
	virtual void SetVisibility(const EVisibility visibility, const bool propagateToChildren = false);
	virtual void SetMobility(const EMobility mobility);

	void SetParent(const UPtr<KSceneComponent>& parent, const ECoordinateSpace keepTransform);

	void SetRelativePosition(const glm::vec3& relativePosition);
	void SetRelativeRotation(const glm::quat& relativeRotation);
	void SetRelativeScale(const glm::vec3& relativeScale);

	void SetWorldPosition(const glm::vec3& worldPosition);
	void SetWorldRotation(const glm::quat& worldRotation);
	void SetWorldScale(const glm::vec3& worldScale);

	void Translate(const glm::vec3& offset);
	void Rotate(const glm::quat& rotation);
	void Scale(const glm::vec3& scale);

	glm::vec3 GetDirection(const UPtr<KSceneComponent>& target) const;
	float GetDistance(const UPtr<KSceneComponent>& other) const;

	virtual void Spawn();

	void Serialize() const override;
	void Deserialize() override;

private:
	bool canUpdate_;
	UPtr<TSceneObject> owner_;
	UPtr<KSceneComponent> parent_;
	SERIALIZE KtPool<UPtr<KSceneComponent>> children_;
	SERIALIZE UTransform transform_;
	SERIALIZE EVisibility visibility_;
	SERIALIZE EMobility mobility_;
	KtEvent<> eventTransformUpdated_; // todo: maybe replace by eventPositionChanged...
									  // dirty children matrix in setposition...
	SERIALIZE size_t componentIndex_;
	SERIALIZE size_t childrenIndex_;
	KtCached<glm::mat4> modelMatrix_;
};

