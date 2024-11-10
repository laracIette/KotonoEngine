#pragma once
#include "BaseClass.h"
class Object
{
public:
    Object();

    virtual void Init();
    virtual void Update(float deltaTime);

    bool GetIsUpdate() const;
    void SetIsUpdate(const bool isUpdate);

    bool GetIsDelete() const;
    void SetIsDelete(const bool isDelete);

private:
    bool _isUpdate;
    bool _isDelete;
};

