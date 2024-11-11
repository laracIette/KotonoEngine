#pragma once
#include <iostream>
#include <string>
#include "BaseClass.h"

class Object
{
public:
    Object();

    virtual void Init();
    virtual void Update(float deltaTime);

    bool GetIsUpdate() const;
    void SetIsUpdate(bool isUpdate);

    bool GetIsDelete() const;
    void SetIsDelete(bool isDelete);

    const std::string& GetName() const;
    void SetName(const std::string& name);

    virtual std::string ToString() const;

private:
    bool _isUpdate;
    bool _isDelete;

    std::string _name;
};

