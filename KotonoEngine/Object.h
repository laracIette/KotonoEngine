#pragma once
#include <iostream>
#include <string>
#include <typeinfo>
#include <format>
#include <vector>
#include "BaseClass.h"

class Object
{
public:
    Object();

    const std::string& GetName() const;
    const std::type_info& GetType() const;

    void SetName(const std::string& name);

    virtual const std::string ToString() const;

private:
    std::string _name;
};

