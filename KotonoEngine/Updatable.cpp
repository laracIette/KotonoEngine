#include "Updatable.h"


Updatable::Updatable() :
    _isDelete(false),
    _isUpdate(true)
{
}

const bool Updatable::GetIsDelete() const
{
    return _isDelete;
}

const bool Updatable::GetIsUpdate() const
{
    return _isUpdate;
}

void Updatable::SetIsDelete(const bool isDelete)
{
    _isDelete = isDelete;
}

void Updatable::SetIsUpdate(const bool isUpdate)
{
    _isUpdate = isUpdate;
}

void Updatable::Init()
{
}

void Updatable::Update(const float deltaTime)
{
}
