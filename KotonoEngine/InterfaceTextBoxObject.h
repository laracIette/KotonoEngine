#pragma once
#include "InterfaceObject.h"

class KInterfaceTextComponent;
class KInterfaceColliderComponent;

class RInterfaceTextBoxObject : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
	void Construct() override;
	void Init() override;
	void Update() override;
	void Cleanup() override;

	const bool GetIsEditable() const;

	void SetIsEditable(const bool isEditable);

	KInterfaceTextComponent* GetTextComponent() const;

protected:
	virtual void OnMousePressed();
	virtual void OnMouseReleased();
	virtual void OnMouseDown();

private:
	KInterfaceTextComponent* textComponent_;
	KInterfaceColliderComponent* colliderComponent_;
	bool isEditable_;
};

