#pragma once
#include "generated/InterfaceBoxComponent.generated.h"
#include "InterfaceComponent.h"
class KInterfaceBoxComponent : public KInterfaceComponent
{
	GENERATED_KINTERFACEBOXCOMPONENT()

public:
	KInterfaceBoxComponent(UPtrOwnerBase* ptrOwner);
	
protected:
	void Cleanup() override;

	void Init() override;

private:
	void CreateBoxProxy();
	void MarkBoxProxyColorDirty();
	void MarkBoxProxyRectDirty();

private:
	KtInterfaceRenderableProxy* boxProxy_;
};

