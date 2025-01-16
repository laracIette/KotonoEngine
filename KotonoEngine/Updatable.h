#pragma once
#include "Object.h"
class Updatable :
    public Object
{
    BASECLASS(Object)

public:
    Updatable();

    const bool GetIsDelete() const;
    const bool GetIsUpdate() const;

    void SetIsDelete(const bool isDelete);
    void SetIsUpdate(const bool isUpdate);

    virtual void Init();
    virtual void Update(const float deltaTime);

private:
    bool _isDelete;
    bool _isUpdate;
};

