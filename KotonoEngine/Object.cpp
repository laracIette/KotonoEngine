#include "Object.h"
#include "KotonoEngine.h"

Object::Object() : 
    _isUpdate(true), 
    _isDelete(false)
{
    Engine->GetWindow()->GetObjectManager()->Create(this);
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

void Object::SetIsUpdate(bool isUpdate)
{
    _isUpdate = isUpdate;
}

bool Object::GetIsDelete() const
{
    return _isDelete;
}

void Object::SetIsDelete(bool isDelete)
{
    _isDelete = isDelete;
}

const std::string& Object::GetName() const
{
    return _name;
}

void Object::SetName(const std::string& name)
{
    _name = name;
}

std::string Object::ToString() const
{
    return "";
}
