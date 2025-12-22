#pragma once
#include "generated/InterfaceBoxComponent.generated.h"
#include "InterfaceComponent.h"
class KInterfaceBoxComponent : public KInterfaceComponent
{
	GENERATED_KINTERFACEBOXCOMPONENT()

public:
	KInterfaceBoxComponent(UPtrOwnerBase* ptrOwner);
	
protected:
	void Init() override;
	void Cleanup() override;

private:
	KtInterfaceRenderableProxy* boxProxy_;

	void CreateBoxProxy();
	void MarkBoxProxyColorDirty();
	void MarkBoxProxyRectDirty();
};

