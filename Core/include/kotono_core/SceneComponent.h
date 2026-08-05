#pragma once
#include "generated/SceneComponent.generated.h"
#include <kotono_object/Object.h>
#include "CoordinateSpace.h"
#include "Transform.h"
#include <kotono_graphics/Visibility.h>
#include <kotono_common/Event.h>
#include <kotono_common/Pool.h>
#include <kotono_graphics/Mobility.h>

class URenderContext;
class TSceneObject;

class KSceneComponent : public KObject
{
	GENERATED_KSCENECOMPONENT()

private:
	friend class TSceneObject;

public:
	KSceneComponent();
	~KSceneComponent() override;

protected:
	virtual void Init();
	virtual void Update(const float deltaTime);

public:
	const UPtr<TSceneObject>& GetOwner() const;
	const UPtr<KSceneComponent>& GetParent() const;
	bool GetCanUpdate() const;
	const UTransform& GetTransform() const;
	EVisibility GetVisibility() const;
	EMobility GetMobility() const;
	bool CanSetTransform() const;
	UEvent<>& GetEventTransformUpdated();

	const glm::vec3& GetRelativePosition() const;
	const glm::quat& GetRelativeRotation() const;
	const glm::vec3& GetRelativeScale() const;

	glm::vec3 GetWorldPosition() const;
	glm::quat GetWorldRotation() const;
	glm::vec3 GetWorldScale() const;

	glm::vec3 RightVector() const;
	glm::vec3 UpVector() const;
	glm::vec3 ForwardVector() const;

	glm::mat4 TranslationMatrix() const;
	glm::mat4 RotationMatrix() const;
	glm::mat4 ScaleMatrix() const;
	glm::mat4 ModelMatrix() const;

	glm::vec3 GetScreenPosition() const;

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

	glm::vec3 GetDirection(const UPtr<const KSceneComponent>& target) const;
	float GetDistance(const UPtr<const KSceneComponent>& other) const;

	void Deserialize() override;

	virtual void Spawn();

	URenderContext& GetRenderContext() const;

private:
	void AddChild(const UPtr<KSceneComponent>& component);
	void RemoveChild(const UPtr<KSceneComponent>& component);

private:
	bool isInit_;
	bool canUpdate_;
	UPtr<TSceneObject> owner_;
	UPtr<KSceneComponent> parent_;
	SERIALIZE UPool<UPtr<KSceneComponent>> children_;
	SERIALIZE UTransform transform_;
	SERIALIZE EVisibility visibility_;
	SERIALIZE EMobility mobility_;
	SERIALIZE size componentIndex_;
	SERIALIZE size childrenIndex_;
	UEvent<> eventTransformUpdated_;
};

