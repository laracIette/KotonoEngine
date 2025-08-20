#pragma once
#include "InterfaceComponent.h"
class KInterfaceBoxComponent : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

public:
	KInterfaceBoxComponent(RInterfaceObject* owner);
	
protected:
	void Init() override;
	void Cleanup() override;

private:
	KtRenderable2DProxy* boxProxy_;

	void CreateBoxProxy();
	void MarkBoxProxyColorDirty();
	void MarkBoxProxyRectDirty();
};

