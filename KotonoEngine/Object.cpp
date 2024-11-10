#include "Object.h"

Object::Object() : 
    _isUpdate(true), _isDelete(false)
{
}

void Object::Init()
{
}

void Object::Update(float deltaTime)
{
}

bool Object::GetIsUpdate() const
{
    return _isUpdate;
}

void Object::SetIsUpdate(const bool isUpdate)
{
    _isUpdate = isUpdate;
}

bool Object::GetIsDelete() const
{
    return _isDelete;
}

void Object::SetIsDelete(const bool isDelete)
{
    _isDelete = isDelete;
}
