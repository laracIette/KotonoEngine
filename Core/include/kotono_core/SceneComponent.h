#pragma once
#include "generated/SceneComponent.generated.h"
#include "Object.h"

#include "CoordinateSpace.h"
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
	virtual void Update(f32 deltaTime);

public:
	auto GetScene() const -> UScene*;
	auto CanSetTransform() const -> b8;

	auto GetWorldPosition() const -> glm::vec3;
	auto GetWorldRotation() const -> glm::quat;
	auto GetWorldScale() const -> glm::vec3;

	auto RightVector() const -> glm::vec3;
	auto UpVector() const -> glm::vec3;
	auto ForwardVector() const -> glm::vec3;

	auto TranslationMatrix() const -> glm::mat4;
	auto RotationMatrix() const -> glm::mat4;
	auto ScaleMatrix() const -> glm::mat4;
	auto ModelMatrix() const -> glm::mat4;

	auto GetScreenPosition() const -> glm::vec3;

	void SetCanUpdate(b8 canUpdate);
	void SetVisibility(EVisibility visibility, b8 propagateToChildren = false);
	void SetMobility(EMobility mobility);

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

	auto GetDirection(UPtr<KSceneComponent const> const& target) const -> glm::vec3;
	auto GetDistance(UPtr<KSceneComponent const> const& other) const -> f32;

	void Deserialize() override;

	virtual void Spawn();
	virtual void Despawn();

	virtual void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const;

	auto GetRelativePosition() const -> glm::vec3 { return transform_.position; }
	auto GetRelativeRotation() const -> glm::quat { return transform_.rotation; }
	auto GetRelativeScale() const -> glm::vec3 { return transform_.scale; }

private:
	void AddChild(UPtr<KSceneComponent> const& component);
	void RemoveChild(UPtr<KSceneComponent> const& component);

private:
	b8 isInit_;
	ReadonlyProperty(b8, canUpdate_, CanUpdate, Value);
	ReadonlyProperty(UPtr<TSceneObject>, owner_, Owner);
	ReadonlyProperty(UPtr<KSceneComponent>, parent_, Parent);
	ReadonlyProperty(UEvent<>, eventTransformUpdated_, EventTransformUpdated, Reference);
	SERIALIZE USet<UPtr<KSceneComponent>> children_;
	SERIALIZE ReadonlyProperty(UTransform, transform_, Transform);
	SERIALIZE ReadonlyProperty(EVisibility, visibility_, Visibility, Value);
	SERIALIZE ReadonlyProperty(EMobility, mobility_, Mobility, Value);
};

