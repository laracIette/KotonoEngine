#pragma once
#include "generated/InterfaceBoxComponent.generated.h"
#include "InterfaceComponent.h"
class KInterfaceBoxComponent : public KInterfaceComponent
{
	GENERATED_KINTERFACEBOXCOMPONENT()

public:
	KInterfaceBoxComponent(UPtrOwnerBase* ptrOwner);
	~KInterfaceBoxComponent() override;
	
protected:
	void Init() override;

public:
	void Spawn() override;

private:
	void CreateBoxProxy();
	void MarkBoxProxyColorDirty();
	void MarkBoxProxyRectDirty();

private:
	UInterfaceProxy* boxProxy_;
};

