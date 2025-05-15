#pragma once
#include "Object.h"
#include "Rect.h"
#include "Visibility.h"

class KtViewport;
class KInterfaceObjectComponent;
class KInterfaceObjectColliderComponent;

class RInterfaceObject : public KObject
{
	BASECLASS(KObject)

public:
	void Construct() override;
	void Init() override;
	void Update() override;
	void Cleanup() override;

	const URect& GetRect() const;
	URect& GetRect();
	const EVisibility GetVisibility() const;
	KtViewport* GetViewport() const; // todo: move that to a renderable interface object component
	RInterfaceObject* GetParent() const;
	RInterfaceObject* GetRoot();
	const int32_t GetLayer() const;
	const std::unordered_set<RInterfaceObject*>& GetChildren() const;
	const bool GetIsSizeToContent() const;

	void SetVisibility(const EVisibility visibility);
	void SetViewport(KtViewport* viewport);
	void SetParent(RInterfaceObject* parent, const ECoordinateSpace keepRect);
	void SetLayer(const int32_t layer);
	void GetIsSizeToContent(const bool isSizeToContent);

private:
	URect rect_;
	EVisibility visibility_;
	KtViewport* viewport_;
	RInterfaceObject* parent_;
	int32_t layer_;
	std::unordered_set<RInterfaceObject*> children_;
	bool isSizeToContent_;
	std::unordered_set<KInterfaceObjectComponent*> components_;
	KInterfaceObjectColliderComponent* collider_;

	void AddBoundsToRenderQueue();

	// temp
	void OnEventColliderMouseLeftButtonDown();
};

