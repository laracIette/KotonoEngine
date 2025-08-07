#pragma once
#include "InterfaceComponent.h"
class KInterfaceContainerComponent : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

protected:
    void Init() override;
    void Update() override;
    void Cleanup() override;

public:
    const KtPool<KInterfaceComponent*>& GetItems() const;
    bool GetIsSizeToContent() const;

    void SetIsSizeToContent(const bool isSizeToContent);
    
    void ClearItems();

    virtual void AddItem(KInterfaceComponent* item);

private:
    KtPool<KInterfaceComponent*> items_;
    bool isSizeToContent_;

    void TryUpdateSize();
    void UpdateSize();
};

