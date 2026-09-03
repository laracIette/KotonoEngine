#pragma once
#include "generated/SceneComponent.generated.h"
#include "CoordinateSpace.h"
#include "Object.h"
#include "Transform.h"
#include <kotono_common/Event.h>
#include <kotono_common/Set.h>
#include <kotono_graphics/Mobility.h>
#include <kotono_graphics/Visibility.h>
struct USceneRenderGraph;
class TSceneObject;
class UScene;
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
	UPtr<TSceneObject> const& GetOwner() const;
	UScene* GetScene() const;
	UPtr<KSceneComponent> const& GetParent() const;
	b8 GetCanUpdate() const;
	UTransform const& GetTransform() const;
	EVisibility GetVisibility() const;
	EMobility GetMobility() const;
	b8 CanSetTransform() const;
	UEvent<>& GetEventTransformUpdated();

	glm::vec3 const& GetRelativePosition() const;
	glm::quat const& GetRelativeRotation() const;
	glm::vec3 const& GetRelativeScale() const;

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

	void SetCanUpdate(b8 canUpdate);
	virtual void SetVisibility(EVisibility visibility, b8 propagateToChildren = false);
	virtual void SetMobility(EMobility mobility);

	void SetParent(UPtr<KSceneComponent> const& parent, ECoordinateSpace keepTransform);

	void SetRelativePosition(glm::vec3 const& relativePosition);
	void SetRelativeRotation(glm::quat const& relativeRotation);
	void SetRelativeScale(glm::vec3 const& relativeScale);

	void SetWorldPosition(glm::vec3 const& worldPosition);
	void SetWorldRotation(glm::quat const& worldRotation);
	void SetWorldScale(glm::vec3 const& worldScale);

	void Translate(glm::vec3 const& offset);
	void Rotate(glm::quat const& rotation);
	void Scale(glm::vec3 const& scale);

	glm::vec3 GetDirection(UPtr<KSceneComponent const> const& target) const;
	f32 GetDistance(UPtr<KSceneComponent const> const& other) const;

	void Deserialize() override;

	virtual void Spawn();

	virtual void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const;

private:
	void AddChild(UPtr<KSceneComponent> const& component);
	void RemoveChild(UPtr<KSceneComponent> const& component);

private:
	b8 isInit_;
	b8 canUpdate_;
	UPtr<TSceneObject> owner_;
	UPtr<KSceneComponent> parent_;
	SERIALIZE USet<UPtr<KSceneComponent>> children_;
	SERIALIZE UTransform transform_;
	SERIALIZE EVisibility visibility_;
	SERIALIZE EMobility mobility_;
	UEvent<> eventTransformUpdated_;
};

